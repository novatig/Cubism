/*
 *  BlockInfo.h
 *  Cubism
 *
 *  Created by Diego Rossinelli on 5/24/09.
 *  Copyright 2009 CSE Lab, ETH Zurich. All rights reserved.
 *
 */
#pragma once

#include <cstdlib>
#include "MeshMap.h"

struct BlockInfo
{
    long long blockID;
    void * ptrBlock;
    bool special;
    int index[3];

    double origin[3];
    double h, h_gridpoint;
    double grid_spacing[3];
    double block_extent[3];

    double* grid_spacing_x;
    double* grid_spacing_y;
    double* grid_spacing_z;

    bool bUniform_X;
    bool bUniform_Y;
    bool bUniform_Z;

///////////////////////////////////////////////////////////////////////////////
    template <typename T>
    inline void pos(T p[2], int ix, int iy) const
    {
        T delta[2] = {0.0};
        if (bUniform_X)
            delta[0] = grid_spacing[0]*(ix+0.5);
        else
        {
            for (int i = 0; i < ix; ++i)
                delta[0] += grid_spacing_x[i];
            delta[0] += 0.5*grid_spacing_x[ix];
        }

        if (bUniform_Y)
            delta[1] = grid_spacing[1]*(iy+0.5);
        else
        {
            for (int i = 0; i < iy; ++i)
                delta[1] += grid_spacing_y[i];
            delta[1] += 0.5*grid_spacing_y[iy];
        }

        p[0] = origin[0] + delta[0];
        p[1] = origin[1] + delta[1];
    }

    template <typename T>
    inline void pos(T p[3], int ix, int iy, int iz) const
    {
        T delta[3] = {0.0};
        if (bUniform_X)
            delta[0] = grid_spacing[0]*(ix+0.5);
        else
        {
            for (int i = 0; i < ix; ++i)
                delta[0] += grid_spacing_x[i];
            delta[0] += 0.5*grid_spacing_x[ix];
        }

        if (bUniform_Y)
            delta[1] = grid_spacing[1]*(iy+0.5);
        else
        {
            for (int i = 0; i < iy; ++i)
                delta[1] += grid_spacing_y[i];
            delta[1] += 0.5*grid_spacing_y[iy];
        }

        if (bUniform_Z)
            delta[2] = grid_spacing[2]*(iz+0.5);
        else
        {
            for (int i = 0; i < iz; ++i)
                delta[2] += grid_spacing_z[i];
            delta[2] += 0.5*grid_spacing_z[iz];
        }

        p[0] = origin[0] + delta[0];
        p[1] = origin[1] + delta[1];
        p[2] = origin[2] + delta[2];
    }
///////////////////////////////////////////////////////////////////////////////

    BlockInfo(long long ID, const int idx[3], const double _pos[3], const double spacing, double h_gridpoint_, void * ptr=NULL, const bool _special=false):
    blockID(ID), ptrBlock(ptr), special(_special),
    grid_spacing_x(NULL), grid_spacing_y(NULL), grid_spacing_z(NULL)
    {
        index[0] = idx[0];
        index[1] = idx[1];
        index[2] = idx[2];

        origin[0] = _pos[0];
        origin[1] = _pos[1];
        origin[2] = _pos[2];

        h = spacing;
        h_gridpoint = h_gridpoint_;

        grid_spacing[0] = h_gridpoint_;
        grid_spacing[1] = h_gridpoint_;
        grid_spacing[2] = h_gridpoint_;

        block_extent[0] = h;
        block_extent[1] = h;
        block_extent[2] = h;

        bUniform_X = true;
        bUniform_Y = true;
        bUniform_Z = true;
    }

    template <typename TBlock>
    BlockInfo(long long ID, const int idx[3], MeshMap<TBlock>* const mapX, MeshMap<TBlock>* const mapY, MeshMap<TBlock>* const mapZ, void * ptr=NULL, const bool _special=false):
    blockID(ID), ptrBlock(ptr), special(_special),
    grid_spacing_x(NULL), grid_spacing_y(NULL), grid_spacing_z(NULL)
    {
        index[0] = idx[0];
        index[1] = idx[1];
        index[2] = idx[2];

        origin[0] = mapX->block_origin(idx[0]);
        origin[1] = mapY->block_origin(idx[1]);
        origin[2] = mapZ->block_origin(idx[2]);

        // TODO: [fabianw@mavt.ethz.ch; Wed May 03 2017 05:06:58 PM (-0700)]
        // ugly but keep this for the moment to ensure that nothing breaks
        // down.
        // WARNING: THIS CAN CAUSE UNEXPECTED RESULTS (if used with a
        // nonuniform grid)!
        h = -1.0;
        h_gridpoint = -1.0;

        block_extent[0] = mapX->block_width(idx[0]);
        block_extent[1] = mapY->block_width(idx[1]);
        block_extent[2] = mapZ->block_width(idx[2]);

        grid_spacing_x = mapX->get_grid_spacing(idx[0]);
        grid_spacing_y = mapY->get_grid_spacing(idx[1]);
        grid_spacing_z = mapZ->get_grid_spacing(idx[2]);

        bUniform_X = mapX->uniform();
        bUniform_Y = mapY->uniform();
        bUniform_Z = mapZ->uniform();

        if (bUniform_X) grid_spacing[0] = grid_spacing_x[0];
        else            grid_spacing[0] = -1.0;
        if (bUniform_Y) grid_spacing[1] = grid_spacing_y[0];
        else            grid_spacing[1] = -1.0;
        if (bUniform_Z) grid_spacing[2] = grid_spacing_z[0];
        else            grid_spacing[2] = -1.0;
    }

    BlockInfo():blockID(-1), ptrBlock(NULL) {}
};
