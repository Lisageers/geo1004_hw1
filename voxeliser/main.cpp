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
  const char *file_in = "../bag_bk.obj";
  const char *file_out = "vox.obj";
  float voxel_size = 1.0;
  
  // Read file
  std::vector<Point> vertices;
  std::vector<std::vector<unsigned int>> faces;
  std::string line;
  float x, y, z;

  std::ifstream input(file_in);
  while(std::getline(input, line))
  {
//      std::cout << line[0] << "\n";
      if (line[0] == 'v')
      {
          std::istringstream ss(line);
          std::string word;
          ss >> word;
          ss >> word;
          x = std::stof(word);
          ss >> word;
          y = std::stof(word);
          ss >> word;
          z = std::stof(word);
          vertices.push_back(Point(x, y, z));

      }
      else if (line[0] == 'f')
      {
          std::istringstream ss(line);
          std::string word;
          std::vector<unsigned int> current_face;
          ss >> word;
          ss >> word;
          current_face.push_back(std::stoi(word));
          ss >> word;
          current_face.push_back(std::stoi(word));
          ss >> word;
          current_face.push_back(std::stoi(word));
          faces.push_back(current_face);
      }
  }
  input.close();
  
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
