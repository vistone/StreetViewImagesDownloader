#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "panorama.h"
#include "download.h"
#include "readXml.h"
#include "readFile.h"
#include "image.h"

int main(int argc, char* argv[])
{


    Image img = getImage("http://cbk0.google.com/cbk?output=tile&panoid=UUIQMV0oHJoFVbx0SxqLeA&zoom=3&x=0&y=0",512,512);
    img.write("test.jpg");
    return 0;

  if(argc!=2){
    std::cout << "Not enough arguments" << std::endl;
  }

  std::ifstream ifs ( argv[1] , std::ifstream::in );

  if(!ifs.good())
    return 1;

  std::string val;
  ifs >> val;
  ifs.close();

  std::vector<Point> list;
  if(val=="latitude"){
    list = readTrajectoryFile(argv[1]);
  }else if(val=="latitudestart"){
    list = readAreaFile(argv[1]);
  }else{
    std::cout << "Incorrect file header" << std::endl;
    return 1;
  }

  if(list.size()==0)
    return 2;


  std::vector<std::string> listXml;
  for(int i=0;i<list.size();i++){
    std::stringstream ss;
    ss << "http://cbk0.google.com/cbk?output=xml&ll=" << list.at(i).getLatitude() << "," << list.at(i).getLongitude();
    listXml.push_back( ss.str() );
  }

  std::vector<Panorama> listPano;
  std::cout << "Download XML files" << std::endl;
  for(int i=0;i<listXml.size();i++){
      //downloadFile(listXml.at(i),"test.xml");
      std::string data = get(listXml.at(i));
      xmlpp::DomParser parser;
      //We can have the text resolved/unescaped automatically.
      parser.set_substitute_entities(true);
      parser.parse_memory(data);
      if(parser){
        //Walk the tree:
        xmlpp::Node* pNode = parser.get_document()->get_root_node(); //deleted by DomParser.
        Panorama pano;
        getPanorama(pNode,0,&pano);
        listPano.push_back(pano);
      }
  }

  std::cout << "Download Panoramas" << std::endl;
  for(int i=0;i<listPano.size();i++){
      Panorama pano = listPano.at(i);
      bool forceZoom = false;
      int zoom = (forceZoom&&pano.num_zoom_levels>2)?2:pano.num_zoom_levels;
      int nbx_=0, nby_=0, croppedwidth_=0, croppedheight_=0;
      switch(zoom)
      {
      case 1:
          nbx_=2;
          nby_=1;
          croppedwidth_=836;
          croppedheight_=416;
          break;
      case 2:
          nbx_=4;
          nby_=2;
          croppedwidth_=1664;
          croppedheight_=832;
          break;
      case 3:
          nbx_=7;
          nby_=4;
          croppedwidth_=nbx_*512-256;
          croppedheight_=nby_*512-384;
          break;
      }

      //ImagePainter *imagePainter = new ImagePainter(num,imagePath,512*nbx_,512*nby_,croppedwidth_,croppedheight_,nbx_*nby_);


      Image imgOut(512*nbx_,512*nby_);
      for(int x=0;x<nbx_;x++){
          for(int y=0;y<nby_;y++){
              std::stringstream ss, filename;
              ss << "http://cbk0.google.com/cbk?output=tile&panoid="<<pano.pano_id<<"&zoom="<<zoom<<"&x="<<x<<"&y="<<y;
              std::cout << ss.str() << std::endl;
              filename << pano.pano_id <<"_" <<x<<"_"<<y<<".jpg";
              //downloadFile(ss.str(),filename.str());

              Image img = getImage(ss.str(),512,512);
              img.write(filename.str());
              Pose pose(x,y);
              imgOut.add(img,pose);
          }
      }

      imgOut.crop(0,0,croppedwidth_,croppedheight_);

      std::stringstream ss;
      ss << "Panorama-" << i << ".jpg";
      imgOut.write(ss.str());
  }
  return 0;
}
