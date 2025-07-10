#include "PixelPerfectPolygon.hpp"
#include "../Texture.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <string>

const int PixelPerfectPolygon::dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
const int PixelPerfectPolygon::dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

PolygonCacheKey PixelPerfectPolygon::createCacheKey(
    std::shared_ptr<Texture> texture,
    const Size& targetSize,
    float alphaThreshold,
    float simplificationTolerance) {
    
    PolygonCacheKey key;
    key.texturePath = texture->path.string();
    key.targetWidth = targetSize.width;
    key.targetHeight = targetSize.height;
    key.alphaThreshold = alphaThreshold;
    key.simplificationTolerance = simplificationTolerance;
    
    if (key.texturePath.empty()) {
        key.texturePath = "handle_" + std::to_string(texture->handle);
    }
    
    return key;
}

std::vector<std::vector<float>> PixelPerfectPolygon::createAlphaMap(
    const unsigned char* pixels,
    int width,
    int height,
    int channels) {
    std::vector<std::vector<float>> alphaMap(height, std::vector<float>(width, 0.0f));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * channels;
            float a = 1.0f;
            if (channels == 4) {
                a = pixels[idx + 3] / 255.0f;
            } else if (channels == 3) {
                a = 1.0f;
            } else if (channels == 1) {
                a = pixels[idx] / 255.0f;
            }
            alphaMap[y][x] = a;
        }
    }
    return alphaMap;
}

std::vector<Point> PixelPerfectPolygon::marchingSquares(
    const std::vector<std::vector<float>>& alphaMap,
    int width,
    int height,
    float threshold) {
    std::vector<Point> pts;
    for (int y = 0; y < height - 1; ++y) {
        for (int x = 0; x < width - 1; ++x) {
            float vTL = alphaMap[y][x];
            float vTR = alphaMap[y][x+1];
            float vBR = alphaMap[y+1][x+1];
            float vBL = alphaMap[y+1][x];
            bool tl = vTL >= threshold;
            bool tr = vTR >= threshold;
            bool br = vBR >= threshold;
            bool bl = vBL >= threshold;
            int state = (tl<<3) | (tr<<2) | (br<<1) | bl;
            float centerVal = (vTL + vTR + vBR + vBL) * 0.25f;
            auto interp = [&](float v1, float v2) {
                if (fabs(v2 - v1) < 1e-6f) return 0.5f;
                float t = (threshold - v1) / (v2 - v1);
                return std::clamp(t, 0.0f, 1.0f);
            };
            Point top(x + interp(vTL, vTR), y);
            Point right(x+1, y + interp(vTR, vBR));
            Point bottom(x + interp(vBL, vBR), y+1);
            Point left(x, y + interp(vTL, vBL));
            switch (state) {
                case 1: pts.push_back(left); pts.push_back(bottom); break;
                case 2: pts.push_back(bottom); pts.push_back(right); break;
                case 3: pts.push_back(left); pts.push_back(right); break;
                case 4: pts.push_back(top); pts.push_back(right); break;
                case 5:
                    if (centerVal > threshold) {
                        pts.push_back(top); pts.push_back(right);
                        pts.push_back(bottom); pts.push_back(left);
                    } else {
                        pts.push_back(left); pts.push_back(bottom);
                        pts.push_back(top); pts.push_back(right);
                    }
                    break;
                case 6: pts.push_back(top); pts.push_back(bottom); break;
                case 7: pts.push_back(left); pts.push_back(top); break;
                case 8: pts.push_back(left); pts.push_back(top); break;
                case 9: pts.push_back(top); pts.push_back(bottom); break;
                case 10:
                    if (centerVal > threshold) {
                        pts.push_back(left); pts.push_back(bottom);
                        pts.push_back(top); pts.push_back(right);
                    } else {
                        pts.push_back(top); pts.push_back(right);
                        pts.push_back(bottom); pts.push_back(left);
                    }
                    break;
                case 11: pts.push_back(top); pts.push_back(right); break;
                case 12: pts.push_back(left); pts.push_back(right); break;
                case 13: pts.push_back(bottom); pts.push_back(right); break;
                case 14: pts.push_back(left); pts.push_back(bottom); break;
                default: break;
            }
        }
    }
    if (pts.empty()) return {};
    return convexHull(pts);
}

std::vector<std::vector<Point>> PixelPerfectPolygon::extractPolygons(
    std::shared_ptr<Texture> texture, 
    const Size& targetSize,
    float alphaThreshold,
    float simplificationTolerance) {
    if (!texture || !texture->getPixelData()) {
        return {};
    }

    const unsigned char* pixels = texture->getPixelData();
    int width = texture->getWidth();
    int height = texture->getHeight();
    int channels = 4;

    auto alphaMap = createAlphaMap(pixels, width, height, channels);
    auto distMap = createSignedDistanceMap(alphaMap, alphaThreshold);
    auto contour = marchingSquares(distMap, width, height, 0.0f);
    contour = smoothPolygon(contour, 2);
    contour = approximateSpline(contour, 5);

    std::vector<std::vector<Point>> polygons;
    if (contour.size() >= 3) {
        auto simplified = simplifyPolygon(contour, simplificationTolerance);
        if (simplified.size() >= 3) {
            Size originalSize(static_cast<float>(width), static_cast<float>(height));
            auto scaled = scalePolygon(simplified, originalSize, targetSize);
            auto validated = validateForBox2D(scaled);
            if (validated.size() >= 3) {
                auto hullPoints = convexHull(validated);
                auto offsetPts = offsetPolygon(hullPoints, 0.5f);

                if (offsetPts.size() <= 8) {
                    polygons.push_back(offsetPts);
                } else {
                    polygons = splitPolygonIntoMultiple(offsetPts, 8);
                }
             }
        }
    }
    return polygons;
}

std::vector<std::vector<float>> PixelPerfectPolygon::createSignedDistanceMap(
    const std::vector<std::vector<float>>& alphaMap,
    float threshold) {
    int H = static_cast<int>(alphaMap.size());
    int W = H ? static_cast<int>(alphaMap[0].size()) : 0;
    float maxDist = static_cast<float>(H + W);
    
    std::vector<std::vector<bool>> inside(H, std::vector<bool>(W));
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            inside[y][x] = alphaMap[y][x] >= threshold;

    std::vector<std::vector<float>> distIn(H, std::vector<float>(W, maxDist));
    std::vector<std::vector<float>> distOut(H, std::vector<float>(W, maxDist));
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            if (inside[y][x]) distIn[y][x] = 0.0f;
            else distOut[y][x] = 0.0f;
        }
    }
    
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            if (x > 0) {
                distIn[y][x] = std::min(distIn[y][x], distIn[y][x-1] + 1.0f);
                distOut[y][x] = std::min(distOut[y][x], distOut[y][x-1] + 1.0f);
            }
            if (y > 0) {
                distIn[y][x] = std::min(distIn[y][x], distIn[y-1][x] + 1.0f);
                distOut[y][x] = std::min(distOut[y][x], distOut[y-1][x] + 1.0f);
            }
        }
    }
    for (int y = H - 1; y >= 0; --y) {
        for (int x = W - 1; x >= 0; --x) {
            if (x + 1 < W) {
                distIn[y][x] = std::min(distIn[y][x], distIn[y][x+1] + 1.0f);
                distOut[y][x] = std::min(distOut[y][x], distOut[y][x+1] + 1.0f);
            }
            if (y + 1 < H) {
                distIn[y][x] = std::min(distIn[y][x], distIn[y+1][x] + 1.0f);
                distOut[y][x] = std::min(distOut[y][x], distOut[y+1][x] + 1.0f);
            }
        }
    }
    
    std::vector<std::vector<float>> signedDist(H, std::vector<float>(W));
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            signedDist[y][x] = distIn[y][x] - distOut[y][x];
        }
    }
    return signedDist;
}

std::vector<Point> PixelPerfectPolygon::simplifyPolygon(
    const std::vector<Point>& polygon,
    float tolerance) {

    if (polygon.size() <= 2) {
        return polygon;
    }
    
    std::vector<bool> keep(polygon.size(), false);
    keep[0] = true;
    keep[polygon.size() - 1] = true;
    
    std::function<void(int, int)> simplifyRecursive = [&](int start, int end) {
        if (end - start <= 1) return;
        
        float maxDistance = 0.0f;
        int maxIndex = -1;
        
        for (size_t i = start + 1; i < end; i++) {
            float distance = pointLineDistance(polygon[i], polygon[start], polygon[end]);
            if (distance > maxDistance) {
                maxDistance = distance;
                maxIndex = static_cast<int>(i);
            }
        }
        
        if (maxDistance > tolerance) {
            keep[maxIndex] = true;
            simplifyRecursive(start, maxIndex);
            simplifyRecursive(maxIndex, end);
        }
    };
    
    simplifyRecursive(0, polygon.size() - 1);
    
    std::vector<Point> simplified;
    for (size_t i = 0; i < polygon.size(); i++) {
        if (keep[i]) {
            simplified.push_back(polygon[i]);
        }
    }
    
    return simplified;
}

float PixelPerfectPolygon::pointLineDistance(const Point& point, const Point& lineStart, const Point& lineEnd) {
    
    float A = point.x - lineStart.x;
    float B = point.y - lineStart.y;
    float C = lineEnd.x - lineStart.x;
    float D = lineEnd.y - lineStart.y;
    
    float dot = A * C + B * D;
    float lenSq = C * C + D * D;
    
    if (lenSq == 0.0f) {
        return std::sqrt(A * A + B * B);
    }
    
    float param = dot / lenSq;
    
    float xx, yy;
    if (param < 0) {
        xx = lineStart.x;
        yy = lineStart.y;
    } else if (param > 1) {
        xx = lineEnd.x;
        yy = lineEnd.y;
    } else {
        xx = lineStart.x + param * C;
        yy = lineStart.y + param * D;
    }
    
    float dx = point.x - xx;
    float dy = point.y - yy;
    
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<Point> PixelPerfectPolygon::scalePolygon(
    const std::vector<Point>& polygon,
    const Size& originalSize,
    const Size& targetSize) {

    if (originalSize.width == 0 || originalSize.height == 0) {
        return polygon;
    }
    
    float scaleX = targetSize.width / originalSize.width;
    float scaleY = targetSize.height / originalSize.height;
    
    std::vector<Point> scaled;
    scaled.reserve(polygon.size());
    
    for (const auto& point : polygon) {
        float x = (point.x - originalSize.width * 0.5f) * scaleX;
        float y = (point.y - originalSize.height * 0.5f) * scaleY;
        scaled.emplace_back(x, y);
    }
    
    return scaled;
}

std::vector<Point> PixelPerfectPolygon::validateForBox2D(const std::vector<Point>& polygon) {
    if (polygon.size() < 3) {
        return polygon;
    }
    
    std::vector<Point> validated = polygon;
    
    if (validated.size() > 8) {

        std::vector<Point> reduced;
        reduced.reserve(8);
        
        float step = static_cast<float>(validated.size()) / 8.0f;
        for (int i = 0; i < 8; i++) {
            int index = static_cast<int>(i * step);
            if (index < validated.size()) {
                reduced.push_back(validated[index]);
            }
        }
        validated = reduced;
    }
    
    float area = 0.0f;
    for (size_t i = 0; i < validated.size(); i++) {
        size_t j = (i + 1) % validated.size();
        area += (validated[j].x - validated[i].x) * (validated[j].y + validated[i].y);
    }
    
    if (area > 0) {
        std::reverse(validated.begin(), validated.end());
    }
    
    return validated;
}

std::vector<std::vector<Point>> PixelPerfectPolygon::splitPolygonIntoMultiple(
    const std::vector<Point>& polygon, 
    int maxVertices) {

    std::vector<std::vector<Point>> result;
    
    if (static_cast<int>(polygon.size()) <= maxVertices) {
        result.push_back(polygon);
        return result;
    }
    
    int numPolygons = (static_cast<int>(polygon.size()) + maxVertices - 1) / maxVertices;
    int verticesPerPolygon = static_cast<int>(polygon.size()) / numPolygons;
    
    for (int i = 0; i < numPolygons; i++) {
        std::vector<Point> subPolygon;
        
        int startIndex = i * verticesPerPolygon;
        int endIndex = (i == numPolygons - 1) ? static_cast<int>(polygon.size()) : (i + 1) * verticesPerPolygon;
        
        float centerX = 0.0f, centerY = 0.0f;
        for (const auto& vertex : polygon) {
            centerX += vertex.x;
            centerY += vertex.y;
        }
        centerX /= static_cast<float>(polygon.size());
        centerY /= static_cast<float>(polygon.size());
        
        Point center(centerX, centerY);
        
        subPolygon.push_back(center);
        for (int j = startIndex; j < endIndex && subPolygon.size() < static_cast<size_t>(maxVertices); j++) {
            subPolygon.push_back(polygon[j]);
        }
        
        if (subPolygon.size() < static_cast<size_t>(maxVertices) && endIndex < static_cast<int>(polygon.size())) {
            subPolygon.push_back(polygon[endIndex % polygon.size()]);
        }
        
        if (subPolygon.size() >= 3) {
            result.push_back(validateForBox2D(subPolygon));
        }
    }
    
    return result;
}

float PixelPerfectPolygon::cross(const Point& O, const Point& A, const Point& B) {
     return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
 }

 std::vector<Point> PixelPerfectPolygon::convexHull(const std::vector<Point>& points) {
     auto P = points;
     if (P.size() <= 1) return P;
     std::sort(P.begin(), P.end(), [](const Point& a, const Point& b) {
         return a.x < b.x || (a.x == b.x && a.y < b.y);
     });
     std::vector<Point> H;
     H.reserve(P.size() * 2);
     for (const auto& pt : P) {
         while (H.size() >= 2 && cross(H[H.size()-2], H[H.size()-1], pt) <= 0) {
             H.pop_back();
         }
         H.push_back(pt);
     }
     size_t t = H.size() + 1;
     for (int i = static_cast<int>(P.size()) - 2; i >= 0; --i) {
         const auto& pt = P[i];
         while (H.size() >= t && cross(H[H.size()-2], H[H.size()-1], pt) <= 0) {
             H.pop_back();
         }
         H.push_back(pt);
     }
     H.pop_back();
     return H;
 }

std::vector<Point> PixelPerfectPolygon::offsetPolygon(const std::vector<Point>& polygon, float offsetDistance) {
    size_t n = polygon.size();
    if (n < 3) return polygon;
    std::vector<Point> result(n);
    for (size_t i = 0; i < n; ++i) {
        const Point& p0 = polygon[(i + n - 1) % n];
        const Point& p1 = polygon[i];
        const Point& p2 = polygon[(i + 1) % n];
        Point v1(p1.x - p0.x, p1.y - p0.y);
        Point v2(p2.x - p1.x, p2.y - p1.y);
        Point n1(-v1.y, v1.x);
        float l1 = std::sqrt(n1.x * n1.x + n1.y * n1.y);
        if (l1 != 0) { n1.x /= l1; n1.y /= l1; }
        Point n2(-v2.y, v2.x);
        float l2 = std::sqrt(n2.x * n2.x + n2.y * n2.y);
        if (l2 != 0) { n2.x /= l2; n2.y /= l2; }
        Point na((n1.x + n2.x) * 0.5f, (n1.y + n2.y) * 0.5f);
        float la = std::sqrt(na.x * na.x + na.y * na.y);
        if (la != 0) { na.x /= la; na.y /= la; }
        result[i] = Point(p1.x + na.x * offsetDistance, p1.y + na.y * offsetDistance);
    }
    return result;
}

std::vector<Point> PixelPerfectPolygon::smoothPolygon(const std::vector<Point>& polygon, int iterations) {
    if (polygon.size() < 2 || iterations <= 0) return polygon;
    std::vector<Point> result = polygon;
    for (int it = 0; it < iterations; ++it) {
        std::vector<Point> temp;
        size_t n = result.size();
        temp.reserve(n * 2);
        for (size_t i = 0; i < n; ++i) {
            const Point& p0 = result[i];
            const Point& p1 = result[(i + 1) % n];
            Point Q(p0.x * 0.75f + p1.x * 0.25f, p0.y * 0.75f + p1.y * 0.25f);
            Point R(p0.x * 0.25f + p1.x * 0.75f, p0.y * 0.25f + p1.y * 0.75f);
            temp.push_back(Q);
            temp.push_back(R);
        }
        result.swap(temp);
    }
    return result;
}

// Approximate polygon with Catmull–Rom spline and resample to polyline
std::vector<Point> PixelPerfectPolygon::approximateSpline(const std::vector<Point>& polygon, int subdivisions) {
    if (polygon.size() < 2 || subdivisions < 1) return polygon;
    std::vector<Point> result;
    size_t n = polygon.size();
    for (size_t i = 0; i < n; ++i) {
        const Point& p0 = polygon[(i + n - 1) % n];
        const Point& p1 = polygon[i];
        const Point& p2 = polygon[(i + 1) % n];
        const Point& p3 = polygon[(i + 2) % n];
        for (int j = 0; j < subdivisions; ++j) {
            float t = static_cast<float>(j) / subdivisions;
            float t2 = t * t;
            float t3 = t2 * t;
            float b0 = 0.5f * (-t3 + 2*t2 - t);
            float b1 = 0.5f * (3*t3 - 5*t2 + 2);
            float b2 = 0.5f * (-3*t3 + 4*t2 + t);
            float b3 = 0.5f * (t3 - t2);
            float x = p0.x * b0 + p1.x * b1 + p2.x * b2 + p3.x * b3;
            float y = p0.y * b0 + p1.y * b1 + p2.y * b2 + p3.y * b3;
            result.emplace_back(x, y);
        }
    }
    return result;
}
