#ifndef MIPT2024S_3_JUNCTION_H
#define MIPT2024S_3_JUNCTION_H

#include <opencv2/opencv.hpp>

class JunctionMat : public cv::Mat {
 public:
  JunctionMat(int radius_border = 15, int angle_border = 105);
  JunctionMat(const cv::Mat& mat, int radius_border = 15,
              int angle_border = 105);
  JunctionMat(const std::string& filename, int radius_border = 15,
              int angle_border = 105);

  std::vector<cv::Point> FindJunctionPoints();
  void Draw();
  void Save(const std::string& filename);

  ~JunctionMat() = default;

 private:
  int radius_border_;
  int angle_border_;
  std::vector<cv::Point> borderPoints_;

  cv::Point GetNormVector(const cv::Point& p, const cv::Point& p1,
                          const cv::Point& p2);
  bool SameHalfPlane(const cv::Point& norm, const cv::Point& v1,
                     const cv::Point& v2);

  void FindBorderPoints();
  bool IsJunction(double angle, const std::vector<cv::Point>& result,
                  cv::Point p);

  void DrawDividers(std::vector<cv::Point> result,
                    std::vector<std::vector<cv::Point>> intersections);
};
#endif  // MIPT2024S_3_JUNCTION_H
