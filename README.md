<p align="right">
  <b>English</b> · <a href="./README_RU.md">Русский</a>
</p>

# Spatial Queries with R-Tree Indexing

Academic algorithms/systems project implementing spatial intersection queries over building geometries with an R-tree index.

The program reads building polygons from a shapefile, converts each geometry to a bounding rectangle, inserts the rectangles into a Boost.Geometry R-tree, and returns object identifiers whose bounding boxes intersect a requested query rectangle.

## Tech

C++ · Boost.Geometry · Boost R-tree · GDAL/OGR · spatial indexing · shapefiles

## Pipeline

1. read a query rectangle from the input file;
2. load `building-polygon.shp` through GDAL/OGR;
3. extract an envelope for every geometry;
4. insert `(bounding rectangle, object id)` pairs into an R-tree using a quadratic split strategy;
5. execute an `intersects` spatial query;
6. sort and write the matching object identifiers to the output file.

## Why R-tree

A linear scan checks every spatial object for every query. An R-tree groups nearby bounding rectangles hierarchically, allowing large parts of the dataset to be skipped when their bounding regions cannot intersect the query window.

## Repository

- `R_Tree/R_Tree.cpp` — complete indexing and query pipeline;
- `R_Tree/data/` — spatial data used by the project;
- `R_Tree/input/` and `R_Tree/output/` — example query and result files;
- `R_Tree.sln` — Visual Studio solution.

## Project context

This is an archived academic implementation focused on spatial indexing and systems-level work with geospatial data rather than a production GIS application.
