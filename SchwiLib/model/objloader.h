#pragma once

#include<vector>
#include<string>
#include<iostream>
#include<sstream>
#include<memory>
#include<map>
#include<math/point2.h>
#include<math/point3.h>
#include<math/normal3.h>

#pragma warning(disable : 4267)

namespace schwi {
	class Model {
	public:
		std::vector<Point3d> vert;
		std::vector<Point2d> tex;
		std::vector<Normal3d> norm;
		std::vector<int> facet_vert;
		std::vector<int> facet_tex;
		std::vector<int> facet_norm;
        std::map<std::string, std::shared_ptr<SchwiImage>> texture_map;

	public:
		Model(const std::string& filename) {
            load_model(filename);
		}

        void load_texture(std::string& name,const std::shared_ptr<SchwiImage>& img) {
            texture_map.emplace(name, img);
        }

        int nverts() const {
            return vert.size();
        }

        int nfaces() const {
            return facet_vert.size() / 3;
        }

        void info(const std::string& filename)const {
            std::cerr << "ObjLoader: [" << filename << "] # v# " << nverts() << " f# " << nfaces() << " vt# " << tex.size() << " vn# " << norm.size() << std::endl;

        }

	private:
		void load_model(const std::string& filename) {
			std::ifstream in;
			in.open(filename, std::ifstream::in);
			if (in.fail()) return;
            std::string line;
            while (!in.eof()) {
                std::getline(in, line);
                std::istringstream iss(line.c_str());
                char trash;
                if (!line.compare(0, 2, "v ")) {
                    iss >> trash;
                    Point3d v;
                    for (int i = 0; i < 3; i++) iss >> v[i];
                    vert.push_back(v);
                }
                else if (!line.compare(0, 3, "vn ")) {
                    iss >> trash >> trash;
                    Normal3d n;
                    for (int i = 0; i < 3; i++) iss >> n[i];
                    norm.push_back(n.Normalize());
                }
                else if (!line.compare(0, 3, "vt ")) {
                    iss >> trash >> trash;
                    Point2d uv;
                    for (int i = 0; i < 2; i++) iss >> uv[i];
                    tex.push_back({ uv.x, 1 - uv.y });
                }
                else if (!line.compare(0, 2, "f ")) {
                    int f, t, n;
                    iss >> trash;
                    int cnt = 0;
                    while (iss >> f >> trash >> t >> trash >> n) {
                        facet_vert.push_back(--f);
                        facet_tex.push_back(--t);
                        facet_norm.push_back(--n);
                        cnt++;
                    }
                    if (3 != cnt) {
                        std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
                        in.close();
                        return;
                    }
                }
            }
            in.close();
        }
	};
}