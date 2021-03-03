#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Point.h"
#include "Rows.h"
#include "VoxelGrid.h"

#include <math.h>
#include <algorithm>

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
  const char *file_out = "../vox.obj";
  float voxel_size = 1;
  float half_size = voxel_size/2;
  int boundary_voxels;
  
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
  float minx, miny, minz, maxx, maxy, maxz;
  for (int i; i < vertices.size(); i++)
  {
      // update min x if found
      if (vertices[i][0] < minx)
      {
          minx = vertices[i][0];
      }
      // update max x if found
      if (vertices[i][0] > maxx)
      {
          maxx = vertices[i][0];
      }
      // update min y if found
      if (vertices[i][1] < miny)
      {
          miny = vertices[i][1];
      }
      // update max y if found
      if (vertices[i][1] > maxy)
      {
          maxy = vertices[i][1];
      }
      // update min z if found
      if (vertices[i][2] < minz)
      {
          minz = vertices[i][2];
      }
      // update max z if found
      if (vertices[i][2] > maxz)
      {
          maxz = vertices[i][2];
      }
  }
  // calculate number of rows in each dimension
  rows.x = ceil((maxx-minx) / voxel_size);
  rows.y = ceil((maxy-miny) / voxel_size);
  rows.z = ceil((maxz-minz) / voxel_size);

  // use number of rows to make the grid
  VoxelGrid voxels(rows.x, rows.y, rows.z);

  // Voxelise
  // initialise
  std::vector<Point> line1, line2, line3;
  std::vector<float> xlist, ylist, zlist;

   // loop through triangles
  for (auto const &triangle: faces)
    {
      int startx, endx, starty, endy, startz, endz;
        // creat lists with x, y and z values of the vertices
        xlist = {vertices[triangle[0]-1][0], vertices[triangle[1]-1][0], vertices[triangle[2]-1][0]};
        ylist = {vertices[triangle[0]-1][1], vertices[triangle[1]-1][1], vertices[triangle[2]-1][1]};
        zlist = {vertices[triangle[0]-1][2], vertices[triangle[1]-1][2], vertices[triangle[2]-1][2]};

        // calculate the start and end voxel numbers in all dimensions for the bounding box
        startx = floor((std::min({xlist[0], xlist[1], xlist[2]}) - minx) / voxel_size);
        endx = ceil((std::max({xlist[0], xlist[1], xlist[2]}) - minx) / voxel_size);
        starty = floor((std::min({ylist[0], ylist[1], ylist[2]}) - miny) / voxel_size);
        endy = ceil((std::max({ylist[0], ylist[1], ylist[2]}) - miny) / voxel_size);
        startz = floor((std::min({zlist[0], zlist[1], zlist[2]}) - minz) / voxel_size);
        endz = ceil((std::max({zlist[0], zlist[1], zlist[2]}) - minz) / voxel_size);

        // calculate ranges
        int xdiff = endx-startx;
        int ydiff = endy-starty;
        int zdiff = endz-startz;

        // loop though bounding box
        for (int ziter = 0; ziter <= zdiff; ziter++)
        {
            for (int yiter = 0; yiter <= ydiff; yiter++)
            {
                for (int xiter = 0; xiter <= xdiff; xiter++)
                {
                    // calculate coordinates of the voxel
                    float xvoxel = (xiter + startx) * voxel_size + minx;
                    float yvoxel = (yiter + starty) * voxel_size + miny;
                    float zvoxel = (ziter + startz) * voxel_size + minz;

                    // create intersection targets, start and end points
                    line1 = {Point(xvoxel, yvoxel+half_size, zvoxel+half_size), Point(xvoxel + voxel_size, yvoxel + half_size, zvoxel+half_size)};
                    line2 = {Point(xvoxel + half_size, yvoxel, zvoxel+half_size), Point(xvoxel + half_size, yvoxel + voxel_size, zvoxel+half_size)};
                    line3 = {Point(xvoxel + half_size, yvoxel + half_size, zvoxel), Point(xvoxel + half_size, yvoxel + half_size, zvoxel+voxel_size)};


                    // check if triangle intersects any of the intersection targets, make voxel 1 if intersect
                    if (intersects(line1[0], line1[1], vertices[triangle[0]-1], vertices[triangle[1]-1], vertices[triangle[2]-1]))
                    {
                        voxels((xiter + startx), (yiter + starty), (ziter + startz)) = 1;
                        boundary_voxels++;
                    }
                     if (intersects(line2[0], line2[1], vertices[triangle[0]-1], vertices[triangle[1]-1], vertices[triangle[2]-1]))
                    {
                        voxels((xiter + startx), (yiter + starty), (ziter + startz)) = 1;
                        boundary_voxels++;
                    }
                    else if (intersects(line3[0], line3[1], vertices[triangle[0]-1], vertices[triangle[1]-1], vertices[triangle[2]-1]))
                    {
                        voxels((xiter + startx), (yiter + starty), (ziter + startz)) = 1;
                        boundary_voxels++;
                    }
                }
            }
        }
    }

  // Fill model
    int exterior_voxels;
  // loop through voxels
    for (int i = 0; i < rows.x; i++)
    {
        for (int j = 0; j < rows.y; j++)
        {
            for (int k = rows.z-1; k >= 0; k--)
            {
                // update voxel when not boundary
                if (voxels (i, j, k) != 1)
                {
                    voxels (i, j, k) = 2;
                    exterior_voxels ++;
                }
                else
                    {
                    break;
                }
            }
        }
    }

    // calculate volume
    int interior_voxels = voxels.voxels.size() - exterior_voxels - boundary_voxels;
    float volume = (boundary_voxels * pow(voxel_size, 3) * 0.5) + (interior_voxels * pow(voxel_size, 3));
    std::cout << "The volume is: " << volume << "\n";

    // Write voxels
    int linecounter = 0;
    float scale = voxel_size * 0.8;
    std::ofstream myfile(file_out);

    // write vertices
    for (int zwrite = 0; zwrite < rows.z; zwrite++)
    {
        for (int ywrite = 0; ywrite < rows.y; ywrite++)
        {
            for (int xwrite = 0; xwrite < rows.x; xwrite++)
            {
                // write if voxel boundary or interior
                if (voxels(xwrite, ywrite, zwrite) == 0 or voxels(xwrite, ywrite, zwrite) == 1)
                {
                    myfile << "v ";
                    myfile << xwrite * voxel_size + minx + (voxel_size- scale) << " ";
                    myfile << ywrite * voxel_size + miny + (voxel_size- scale) << " ";
                    myfile << zwrite * voxel_size + minz + (voxel_size- scale) << "\n";
                    linecounter++;

                    myfile << "v ";
                    myfile << xwrite * voxel_size + minx + scale << " ";
                    myfile << ywrite * voxel_size + miny + (voxel_size- scale) << " ";
                    myfile << zwrite * voxel_size + minz + (voxel_size- scale) << "\n";
                    linecounter++;

                    myfile << "v ";
                    myfile << xwrite * voxel_size + minx + scale  << " ";
                    myfile << ywrite * voxel_size + miny + scale  << " ";
                    myfile << zwrite * voxel_size + minz + (voxel_size- scale) << "\n";
                    linecounter++;

                    myfile << "v ";
                    myfile << xwrite * voxel_size + minx + (voxel_size- scale) << " ";
                    myfile << ywrite * voxel_size + miny + scale << " ";
                    myfile << zwrite * voxel_size + minz + (voxel_size- scale) << "\n";
                    linecounter++;

                    myfile << "v ";
                    myfile << xwrite * voxel_size + minx + (voxel_size- scale) << " ";
                    myfile << ywrite * voxel_size + miny + (voxel_size- scale) << " ";
                    myfile << zwrite * voxel_size + minz + scale << "\n";
                    linecounter++;

                    myfile << "v ";
                    myfile << xwrite * voxel_size + minx + scale << " ";
                    myfile << ywrite * voxel_size + miny + (voxel_size- scale) << " ";
                    myfile << zwrite * voxel_size + minz + scale << "\n";
                    linecounter++;

                    myfile << "v ";
                    myfile << xwrite * voxel_size + minx + scale << " ";
                    myfile << ywrite * voxel_size + miny + scale << " ";
                    myfile << zwrite * voxel_size + minz + scale << "\n";
                    linecounter++;

                    myfile << "v ";
                    myfile << xwrite * voxel_size + minx + (voxel_size- scale) << " ";
                    myfile << ywrite * voxel_size + miny + scale << " ";
                    myfile << zwrite * voxel_size + minz + scale << "\n";
                    linecounter++;

                }
            }
        }
    }

    // write faces
    for (int i = 0; i < linecounter; i++)
    {
        myfile << "f ";
        myfile << i+1 << " ";
        myfile << i+2 << " ";
        myfile << i+3 << " ";
        myfile << i+4 << "\n";

        myfile << "f ";
        myfile << i+1 << " ";
        myfile << i+2 << " ";
        myfile << i+6 << " ";
        myfile << i+5 << "\n";

        myfile << "f ";
        myfile << i+5 << " ";
        myfile << i+6 << " ";
        myfile << i+7 << " ";
        myfile << i+8 << "\n";

        myfile << "f ";
        myfile << i+4 << " ";
        myfile << i+3 << " ";
        myfile << i+7 << " ";
        myfile << i+8 << "\n";

        myfile << "f ";
        myfile << i+1 << " ";
        myfile << i+5 << " ";
        myfile << i+8 << " ";
        myfile << i+4 << "\n";

        myfile << "f ";
        myfile << i+2 << " ";
        myfile << i+6 << " ";
        myfile << i+7 << " ";
        myfile << i+3 << "\n";

        i = i + 7;
    }
  
  return 0;
}
