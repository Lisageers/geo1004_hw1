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
  // initialise
  std::vector<Point> vertices;
  std::vector<std::vector<unsigned int>> faces;
  std::string line;
  float x, y, z;

  // read lines
  std::ifstream input(file_in);
  while(std::getline(input, line))
  {
      // if line starts with v
      if (line[0] == 'v')
      {
          // split line at spaces
          std::istringstream ss(line);
          // move to second element
          std::string word;
          ss >> word;
          ss >> word;
          // create x with string converted to float
          x = std::stof(word);
          // move one further to get y
          ss >> word;
          y = std::stof(word);
          // move one further to get z
          ss >> word;
          z = std::stof(word);
          // put vertex point in vector
          vertices.push_back(Point(x, y, z));

      }
      // if line starts with f
      else if (line[0] == 'f')
      {
          // split line at spaces
          std::istringstream ss(line);
          // initialise
          std::string word;
          std::vector<unsigned int> current_face;
          // move two to get first index
          ss >> word;
          ss >> word;
          // convert string to int, then put first index into vector
          current_face.push_back(std::stoi(word));
          // move to second index and to the same
          ss >> word;
          current_face.push_back(std::stoi(word));
          ss >> word;
          current_face.push_back(std::stoi(word));
          // put vector in another vector
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
