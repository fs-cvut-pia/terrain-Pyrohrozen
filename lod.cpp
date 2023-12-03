#include "Path.h"
#include "TerrainMap.h"
#include <queue>
#include <cmath>
#include <iostream>
//#include "TerrainMap.cpp"

using namespace std;

class Lod:public Path {
private:
    Matrix<Point> predchudci = Matrix<Point>(256,256);
    vector<vector<pair<int, double>>> checked_distance;
    // 1=nenalezeny 2=otevreny 3=uzavreny

public:

    Lod(TerrainMap &m, std::string nameIn, Point startIn, Point finishIn)
        :Path(m, nameIn, startIn, finishIn) {

        for(int i = 0; i < 256; i++) {
            vector<pair<int, double>> vec;
            for(int j = 0; j < 256; j++) {
                vec.emplace_back(1, 0);
            }
            checked_distance.push_back(vec);
        }
    }

    bool find() override {
        queue<Point> fronta;
        fronta.push(start);
        checked_distance[start.x][start.y].second=0;
        checked_distance[start.x][start.y].first=2;
        while (!fronta.empty()) {
            Point actual=fronta.front();
            fronta.pop();
            vector<Point> sousedi = getSousedi(actual);
            for (Point& soused : sousedi) {
                if (checked_distance[soused.x][soused.y].first == 1 ) {
                    checked_distance[soused.x][soused.y].first=2;
                    checked_distance[soused.x][soused.y].second = checked_distance[actual.x][actual.y].second+1;
                    Point& idk = predchudci(soused);
                    idk = actual;
                    fronta.push(soused);
                }
                checked_distance[soused.x][soused.y].first=3;
            }

            sousedi=getSousedisikme(actual);
            for (Point& soused: sousedi) {
                if (checked_distance[soused.x][soused.y].first == 1) {
                    checked_distance[soused.x][soused.y].first=2;
                    checked_distance[soused.x][soused.y].second = checked_distance[actual.x][actual.y].second+std::sqrt(2);
                    Point& idk = predchudci(soused);
                    idk = actual;
                    fronta.push(soused);
                }
                checked_distance[soused.x][soused.y].first=3;
            }
        }
        getPath();

        return true;
    }

    void getPath() {
        Point actual=finish;
        while (actual!=start) {
            path.push_back(actual);
            actual=predchudci(actual);

        }
        path.push_back(actual);
    }

    bool isgoodpoint(Point a){
        return map.validCoords(a)&&(map.alt(a.x,a.y)<0||(a.x==finish.x&&a.y==finish.y));
    }

    vector<Point> getSousedisikme(const Point& actual){
        vector<Point> ret;

        Point a = Point(actual.x+1,actual.y+1);
        if (isgoodpoint(a)) {
            ret.push_back(a);
        }

        a = Point(actual.x-1,actual.y+1);
        if (isgoodpoint(a)) {
            ret.push_back(a);
        }

        a = Point(actual.x+1,actual.y-1);
        if (isgoodpoint(a)) {
            ret.push_back(a);
        }

        a = Point(actual.x-1,actual.y-1);
        if (isgoodpoint(a)) {
            ret.push_back(a);
        }

        return ret;
    }

    vector<Point> getSousedi(const Point& actual){
        vector<Point> ret;
        Point a = Point(actual.x+1,actual.y);
        if (isgoodpoint(a)) {
            ret.push_back(a);
        }

        a = Point(actual.x,actual.y+1);
        if (isgoodpoint(a)) {
            ret.push_back(a);
        }

        a = Point(actual.x-1,actual.y);
        if (isgoodpoint(a)) {
            ret.push_back(a);
        }

        a = Point(actual.x,actual.y-1);
        if (isgoodpoint(a)) {
            ret.push_back(a);
        }

        return ret;
    }





};


