#include <iostream>
#include <fstream>
#include "ogrsf_frmts.h"
#include "boost/geometry.hpp"

typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> point;
typedef boost::geometry::model::box<point> rectangle;
typedef std::pair<rectangle, int> variable;
typedef boost::geometry::index::rtree<variable, boost::geometry::index::quadratic<8, 4>> rtree;


int main(int argc, char* argv[])
{
    std::ifstream inp{ argv[2] };

    double x_min;
    inp >> x_min;

    double y_min;
    inp >> y_min;

    point s0(x_min, y_min);

    double x_max;
    inp >> x_max;

    double y_max;
    inp >> y_max;

    point s1(x_max, y_max);

    GDALAllRegister();

    auto poDS = (GDALDataset*)GDALOpenEx((std::string(argv[1]) + std::string("\\building-polygon.shp")).c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);

    if (poDS == nullptr)
    {
        printf("Open failed.\n");
        exit(1);
    }

    rtree rt;

    for (OGRLayer* poLayer : poDS->GetLayers())
    {
        for (auto& poFeature : *poLayer)
        {
            int osmid = poFeature->GetFieldAsInteger(0);

            OGRGeometry* poGeometry = poFeature->GetGeometryRef();

            if (poGeometry == nullptr) continue;

            OGREnvelope* envelope = new OGREnvelope {};
            poGeometry->getEnvelope(envelope);

            point p0(envelope->MinX, envelope->MinY);
            point p1(envelope->MaxX, envelope->MaxY);

            rectangle rect(p0, p1);

            variable vari(rect, osmid);

            rt.insert(vari);
        }
    }

    rectangle query(s0, s1);
    std::vector<variable> result_s;
    rt.query(boost::geometry::index::intersects(query), std::back_inserter(result_s));

    std::vector<int> result_int;

    for (variable result : result_s) {
        result_int.push_back(result.second);
    }

    std::sort(result_int.begin(), result_int.end());

    std::ofstream out{ argv[3] };

    for (int result : result_int) {
        out << result << std::endl;
    }

    return 0;

}

