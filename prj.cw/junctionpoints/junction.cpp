#include "junction.h"

#include <filesystem>
#include <opencv2/opencv.hpp>

bool onSegment(cv::Point p, cv::Point q, cv::Point r) {
  if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
      q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
    return true;

  return false;
}

int orientation(cv::Point p, cv::Point q, cv::Point r) {
  // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
  // for details of below formula.
  int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

  if (val == 0) return 0;  // colinear

  return (val > 0) ? 1 : 2;  // clock or counterclock wise
}

bool doIntersect(cv::Point p1, cv::Point q1, cv::Point p2, cv::Point q2) {
  // Find the four orientations needed for general and
  // special cases
  p1 += (p1 - q1) * 10;
  q1 += (q1 - p1) * 10;
  int o1 = orientation(p1, q1, p2);
  int o2 = orientation(p1, q1, q2);
  int o3 = orientation(p2, q2, p1);
  int o4 = orientation(p2, q2, q1);

  // General case
  if (o1 != o2 && o3 != o4) return true;

  //  // Special Cases
  //  // p1, q1 and p2 are colinear and p2 lies on segment p1q1
  //  if (o1 == 0 && onSegment(p1, p2, q1)) return true;
  //
  //  // p1, q1 and q2 are colinear and q2 lies on segment p1q1
  //  if (o2 == 0 && onSegment(p1, q2, q1)) return true;
  //
  //  // p2, q2 and p1 are colinear and p1 lies on segment p2q2
  //  if (o3 == 0 && onSegment(p2, p1, q2)) return true;
  //
  //  // p2, q2 and q1 are colinear and q1 lies on segment p2q2
  //  if (o4 == 0 && onSegment(p2, q1, q2)) return true;

  return false;  // Doesn't fall in any of the above cases
}

cv::Point CalcIntersection(cv::Point p1, cv::Point q1, cv::Point p2,
                           cv::Point q2) {
  cv::Point pInt(0, 0);
  if (doIntersect(p1, q1, p2, q2)) {
    double x1 = p1.x, y1 = p1.y;
    double x2 = q1.x, y2 = q1.y;
    double x3 = p2.x, y3 = p2.y;
    double x4 = q2.x, y4 = q2.y;

    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    double x =
        ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) /
        denom;
    double y =
        ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) /
        denom;
    pInt.x = x;

    pInt.y = y;
  }
  return pInt;
}

void drawInfiniteLine(cv::Mat& image, cv::Point pt1, cv::Point pt2) {
  pt1 += (pt1 - pt2) * 5;
  pt2 += (pt2 - pt1) * 5;

  // Draw the infinite line passing through the two points
  cv::line(image, pt1, pt2, cv::Scalar(255, 0, 0),
           1);  // White line with thickness 2
}

std::vector<cv::Point> findIntersectionsWithPolygon(
    std::vector<cv::Point> polygon, cv::Point point1, cv::Point point2) {
  // Define the line using the two points provided
  std::vector<cv::Point> line = {point1, point2};

  // Check for intersections with the line
  std::vector<cv::Point> intersectionPoints;
  for (size_t i = 0; i < polygon.size(); ++i) {
    cv::Point p1 = polygon[i];
    cv::Point p2 = (i < polygon.size() - 1) ? polygon[i + 1] : polygon[0];

    cv::Vec4i edge(p1.x, p1.y, p2.x, p2.y);
    //    std::vector<cv::Point> currentIntersectionPoints;
    //    cv::intersectConvexConvex(line, edge, currentIntersectionPoints,
    //    false);
    //
    //    intersectionPoints.insert(intersectionPoints.end(),
    //    currentIntersectionPoints.begin(), currentIntersectionPoints.end());
    auto res = CalcIntersection(point1, point2, p1, p2);
    if (res.x != 0 && res.y != 0) intersectionPoints.push_back(res);
  }

  return intersectionPoints;
}

JunctionMat::JunctionMat(int radius_border, int angle_border)
    : cv::Mat(), radius_border_(radius_border), angle_border_(angle_border) {
  FindBorderPoints();
}

JunctionMat::JunctionMat(const cv::Mat& mat, int radius_border,
                         int angle_border)
    : cv::Mat(mat), radius_border_(radius_border), angle_border_(angle_border) {
  FindBorderPoints();
}

JunctionMat::JunctionMat(const std::string& filename, int radius_border,
                         int angle_border)
    : cv::Mat(cv::imread(filename)),
      radius_border_(radius_border),
      angle_border_(angle_border) {
  FindBorderPoints();
}

std::vector<cv::Point> JunctionMat::FindJunctionPoints() {
  std::vector<cv::Point> result;
  std::vector<size_t> indices;
  std::vector<std::vector<cv::Point>> intersections;

  for (size_t i = 0; i < borderPoints_.size(); ++i) {
    cv::Point p1 =
        borderPoints_[i - radius_border_ >= 0
                          ? i - radius_border_
                          : borderPoints_.size() + i - radius_border_];
    cv::Point p2 = borderPoints_[i];
    cv::Point p3 =
        borderPoints_[i + radius_border_ < borderPoints_.size()
                          ? i + radius_border_
                          : i + radius_border_ - borderPoints_.size()];

    cv::Point norm = GetNormVector(p2, p1 - p2, p3 - p2);

    double angle = std::acos((p1 - p2).dot(p3 - p2) /
                             (cv::norm(p2 - p1) * cv::norm(p3 - p2))) *
                   180 / M_PI;
    //    if (i % 25 == 0) {
    //      cv::arrowedLine(*this, p2, p1, cv::Scalar(0, 255, 0), 1);
    //      cv::arrowedLine(*this, p2, p3, cv::Scalar(0, 255, 255), 1);
    //    }

    if (IsJunction(angle, result, p2) &&
        SameHalfPlane(norm, p1 - p2, p3 - p2)) {
      result.push_back(p2);
      indices.push_back(i);
//      drawInfiniteLine(*this, p1, p3);
      auto cur_inter = findIntersectionsWithPolygon(borderPoints_, p1, p3);
      intersections.push_back(cur_inter);
//      for (const auto& inter : cur_inter) {
//        cv::circle(*this, inter, 5, cv::Scalar(0, 0, 255), 2);
//      }
//      cv::arrowedLine(*
      //      this, p2, p1, cv::Scalar(0, 255, 0), 1);
//      cv::arrowedLine(*this, p2, p3, cv::Scalar(0, 255, 255), 1);
//      cv::arrowedLine(*this, p2, p2 + norm, cv::Scalar(0, 0, 255), 1);
    }
    //    cv::arrowedLine(*this, borderPoints_[i],
    //                    borderPoints_[i > 0 ? i - 1 : borderPoints_.size() -
    //                    1], cv::Scalar(0, 0, 255), 2);
  }

  //  cv::circle(*this, borderPoints_[0], 5, cv::Scalar(255, 0, 0), 2);
  //  cv::circle(*this, borderPoints_[borderPoints_.size() - 1], 5,
  //  cv::Scalar(255, 0, 0), 2);
  //  drawInfiniteLine(*this, {0, 0}, {100, 100});
  //  drawInfiniteLine(*this, {0, 100}, {100, 0});

  //  auto cur_inter = findIntersectionsWithPolygon(borderPoints_, {0, 0}, {100,
  //  100}); for (const auto& inter : cur_inter) {
  //  cv::circle(*this, CalcIntersection({0, 0}, {1, 1}, {0, 100}, {100, 0}), 5,
  //             cv::Scalar(0, 0, 255), 2);
  //  }
  for (const cv::Point& point : result) {
    cv::circle(*this, point, 5, cv::Scalar(255, 0, 0), 2);
  }
  return result;
}

void JunctionMat::Draw() {
  cv::namedWindow("Image Edges", cv::WINDOW_AUTOSIZE);
  cv::imshow("Image Edges", *this);
  cv::waitKey(0);
}

void JunctionMat::FindBorderPoints() {
  cv::Mat edges;
  cv::Canny(*this, edges, 100, 200);

  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
  borderPoints_ = contours[0];
}

void JunctionMat::Save(const std::string& filename) {
  cv::imwrite(filename, *this);
}

bool JunctionMat::IsJunction(double angle, const std::vector<cv::Point>& result,
                             cv::Point p) {
  return angle > 0 && angle < angle_border_ &&
         (result.size() <= 1 || cv::norm(result[result.size() - 1] - p) > 25);
  //  return angle > 0 && angle < angle_border_ &&
  //         (result.size() == 0 || result.back() < p - 300);
}

bool JunctionMat::SameHalfPlane(const cv::Point& norm, const cv::Point& v1,
                                const cv::Point& v2) {
  float crossProduct1 = norm.x * v1.x + norm.y * v1.y;
  float crossProduct2 = norm.x * v2.x + norm.y * v2.y;

  return crossProduct1 > 0 && crossProduct2 > 0;
}

cv::Point JunctionMat::GetNormVector(const cv::Point& p, const cv::Point& p1,
                                     const cv::Point& p2) {
  cv::Point norm(p2 + p1);
//  norm /= cv::norm(norm);
//  norm *= 10;
  //  std::vector<int> xAdd = {0, 1, 2, 3, 4};
  //  std::vector<int> yAdd = {0, 1, 2, 3, 4};
  //  for (auto x : xAdd) {
  //    for (auto y : yAdd) {
  //      norm.x += x;
  //      norm.y += y;
  //      auto color = this->at<cv::Vec3b>(norm.y, norm.x);
  //      if (color[0] == 255 && color[1] == 255 && color[2] == 255) {
  //        return norm - p;
  //      }
  //      norm.x -= x;
  //      norm.y -= y;
  //    }
  //  }
  norm += p;
  if (norm.x >= 0 && norm.y >= 0 && norm.x < this->cols &&
      norm.y < this->rows) {
    auto color = this->at<cv::Vec3b>(norm.y, norm.x);
    if (color[0] == 255 && color[1] == 255 && color[2] == 255) {
      return norm - p;
    }
    norm -= p;
    norm *= -1;
  }
  return norm;
}

void processImagesInFolder(const std::string& folderPath,
                           const std::string& outputFolderPath,
                           const std::string& combinedImagePath) {
  std::vector<JunctionMat> processedImages;
  for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
    if (entry.is_regular_file() && (entry.path().extension() == ".jpg" ||
                                    entry.path().extension() == ".png")) {
      JunctionMat junctionMat(entry.path().string(), 5, 140);

      if (junctionMat.empty()) {
        std::cerr << "Error loading image: " << entry.path().string()
                  << std::endl;
        continue;
      }

      junctionMat.FindJunctionPoints();
      junctionMat.Save(outputFolderPath + entry.path().filename().string());
      processedImages.push_back(junctionMat);
    }
  }
  if (!processedImages.empty()) {
    int numRows = 4;
    int numCols = (processedImages.size() + numRows - 1) / numRows;

    int cellWidth = processedImages[0].cols;
    int cellHeight = processedImages[0].rows;

    cv::Mat combinedImage(cellHeight * numRows, cellWidth * numCols,
                          processedImages[0].type(), cv::Scalar(255, 255, 255));

    for (int i = 0; i < processedImages.size(); ++i) {
      int row = i / numCols;
      int col = i % numCols;

      cv::Rect roi(col * cellWidth, row * cellHeight, cellWidth, cellHeight);
      cv::Mat targetROI = combinedImage(roi);
      cv::resize(processedImages[i], processedImages[i], targetROI.size());
      processedImages[i].copyTo(targetROI);
    }

    std::string outputFileName = combinedImagePath + "combined_result.png";
    cv::imwrite(outputFileName, combinedImage);
  }
}

int main() {
  std::string folderPath = "../prj.cw/images/test/";
  std::string outputFolderPath = "../prj.cw/images/result/";
  std::string combinedImagePath = "../prj.cw/images/resultcombo/";

  processImagesInFolder(folderPath, outputFolderPath, combinedImagePath);

  return 0;
}