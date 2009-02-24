//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    plugin_find_blobs.cpp
  \brief   C++ Implementation: plugin_find_blobs
  \author  Stefan Zickler, 2008
*/
//========================================================================
#include "plugin_find_blobs.h"

PluginFindBlobs::PluginFindBlobs(FrameBuffer * _buffer, YUVLUT * _lut, int _max_regions)
 : VisionPlugin(_buffer)
{
  lut=_lut;
  max_regions=_max_regions;

  _settings=new VarList("Blob Finding");
  _settings->addChild(_v_min_blob_area=new VarInt("min_blob_area", 5));

}


PluginFindBlobs::~PluginFindBlobs()
{
}



ProcessResult PluginFindBlobs::process(FrameData * data, RenderOptions * options) {
  (void)options;


  CMVision::RegionList * reglist;
  if ((reglist=(CMVision::RegionList *)data->map.get("cmv_reglist")) == 0) {
    reglist=(CMVision::RegionList *)data->map.insert("cmv_reglist",new CMVision::RegionList(max_regions));
  }

  CMVision::ColorRegionList * colorlist;
  if ((colorlist=(CMVision::ColorRegionList *)data->map.get("cmv_colorlist")) == 0) {
    colorlist=(CMVision::ColorRegionList *)data->map.insert("cmv_colorlist",new CMVision::ColorRegionList(lut->getChannelCount()));
  }

  CMVision::RunList * runlist;
  if ((runlist=(CMVision::RunList *)data->map.get("cmv_runlist")) == 0) {
    printf("Blob finder: no runlength-encoded input list was found!\n");
    return ProcessingFailed;
  }

  //Connect the components of the runlength map:
  CMVisionRegion::connectComponents(runlist);

  //Extract Regions from runlength map:
  CMVisionRegion::extractRegions(reglist, runlist);

  if (reglist->getUsedRegions() == reglist->getMaxRegions()) {
    printf("Warning: extract regions exceeded maximum number of %d regions\n",reglist->getMaxRegions());
  }

  //Separate Regions by colors:
  CMVisionRegion::separateRegions(colorlist, reglist, _v_min_blob_area->getInt());

  return ProcessingOk;
  
  

}

VarList * PluginFindBlobs::getSettings() {
  return _settings;
}

string PluginFindBlobs::getName() {
  return "FindBlobs";
}