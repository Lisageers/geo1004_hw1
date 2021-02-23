#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Point.h"
#include "Rows.h"
#include "VoxelGrid.h"

float signed_volume(const Point &a, const Point &b, const Point &c, const Point &d) {
  // to do
}

bool intersects(const Point &orig, const Point &dest, const Point &v0, const Point &v1, const Point &v2) {
  // to do
}

int main(int argc, const char * argv[]) {
  const char *file_in = "bag_bk.obj";
  const char *file_out = "vox.obj";
  float voxel_size = 1.0;
  
  // Read file
  std::vector<Point> vertices;
  std::vector<std::vector<unsigned int>> faces;
  // to do
  
  // Create grid
  Rows rows;
  // to do
  VoxelGrid voxels(rows.x, rows.y, rows.z);
  
  // Voxelise
  for (auto const &triangle: faces) {
    // to do
  }
  
  // Fill model
  // to do
  
  // Write voxels
  // to do
  
  return 0;
}
