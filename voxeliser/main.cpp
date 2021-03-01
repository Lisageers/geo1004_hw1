#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Point.h"
#include "Rows.h"
#include "VoxelGrid.h"

#include <math.h>

float signed_volume(const Point &a, const Point &b, const Point &c, const Point &d) {
    Point crossproduct = (b-d).cross((c-d));
    float volume =  (a-d).dot(crossproduct) / 6;
    return volume;
}

bool intersects(const Point &orig, const Point &dest, const Point &v0, const Point &v1, const Point &v2) {
    float volume1 = signed_volume(v0, v1, v2, orig);
    float volume2 = signed_volume(v0, v1, v2, dest);
    if ((volume1 < 0 and volume2 < 0) or (volume1 > 0 and volume2 > 0))
    {
        return false;
    }

    float volume3 = signed_volume(orig, dest, v0, v1);
    float volume4 = signed_volume(orig, dest, v0, v2);
    if ((volume3 < 0 and volume4 < 0) or (volume3 > 0 and volume4 > 0))
    {
        return false;
    }

    float volume5 = signed_volume(orig, dest, v1, v0);
    float volume6 = signed_volume(orig, dest, v1, v2);
    if ((volume5 < 0 and volume6 < 0) or (volume5 > 0 and volume6 > 0))
    {
        return false;
    }

    float volume7 = signed_volume(orig, dest, v2, v0);
    float volume8 = signed_volume(orig, dest, v2, v1);
    if ((volume7 < 0 and volume8 < 0) or (volume7 > 0 and volume8 > 0))
    {
        return false;
    }
    return true;
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
  float maxx, maxy, maxz;
  for (int i; i < vertices.size(); i++)
  {
      // update max x if found
      if (vertices[i][0] > maxx)
      {
          maxx = vertices[i][0];
      }
      // update max y if found
      if (vertices[i][1] > maxy)
      {
          maxy = vertices[i][1];
      }
      // update max z if found
      if (vertices[i][2] > maxz)
      {
          maxz = vertices[i][2];
      }
  }
  // calculate number of rows in each dimension
  rows.x = ceil(maxx / voxel_size);
  rows.y = ceil(maxy / voxel_size);
  rows.z = ceil(maxz / voxel_size);

  // use number of rows to make the grid
  VoxelGrid voxels(rows.x, rows.y, rows.z);


  // Voxelise
  // initialise lines
  std::vector<Point> line1, line2, line3;

//   loop through each voxel
    for (int ziter = 0; ziter < rows.z; ziter++)
    {
        for (int yiter = 0; yiter < rows.y; yiter++)
        {
            for (int xiter = 0; xiter < rows.x; xiter++)
            {
                // create intersection targets
                line1 = {Point(xiter, yiter+0.5, ziter+0.5), Point(xiter + 1, yiter + 0.5, ziter+0.5)};
                line2 = {Point(xiter + 0.5, yiter, ziter+0.5), Point(xiter + 0.5, yiter + 1, ziter+0.5)};
                line3 = {Point(xiter + 0.5, yiter + 0.5, ziter), Point(xiter + 0.5, yiter + 0.5, ziter+1)};

                for (auto const &triangle: faces)
                {
                    if (intersects(line1[0], line1[1], vertices[triangle[0]-1], vertices[triangle[1]-1], vertices[triangle[2]-1]))
                    {
                        voxels(xiter, yiter, ziter) = 1;
                    }
                    else if (intersects(line2[0], line2[1], vertices[triangle[0]-1], vertices[triangle[1]-1], vertices[triangle[2]-1]))
                    {
                        voxels(xiter, yiter, ziter) = 1;
                    }
                    else if (intersects(line3[0], line3[1], vertices[triangle[0]-1], vertices[triangle[1]-1], vertices[triangle[2]-1]))
                    {
                        voxels(xiter, yiter, ziter) = 1;
                    }

                }


            }
        }
    }

// for testing
//    std::vector<Point> line1{Point(0, 0.5, 0.5), Point(1, 0.5, 0.5)};
//    std::vector<Point> line2{Point(0.5, 0, 0.5), Point(0.5, 1, 0.5)};
//    std::vector<Point> line3{Point(0.5, 0.5, 0), Point(0.5, 0.5, 1)};
//    std::vector<Point> line4{Point(50, 50, 50), Point(51, 50, 50)};
//    bool inter = intersects(line4[0], line4[1], Point(0.5, 0.5, 0), Point(0.5, 0.5, 1), Point(0.5, 1, 0.5));
//    std::cout << inter;


  // Fill model
  // to do
  
  // Write voxels
  // to do
  
  return 0;
}
