#include <libxml++/libxml++.h>
#include "panorama.h"

void print_node(const xmlpp::Node* node, unsigned int indentation = 0)
{
  const Glib::ustring indent(indentation, ' ');
  std::cout << std::endl; //Separate nodes by an empty line.

  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
  const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

  if(nodeText && nodeText->is_white_space()) //Let's ignore the indenting - you don't always want to do this.
    return;

  const Glib::ustring nodename = node->get_name();

  if(!nodeText && !nodeComment && !nodename.empty()) //Let's not say "name: text".
  {
    const Glib::ustring namespace_prefix = node->get_namespace_prefix();

    std::cout << indent << "Node name = ";
    if(!namespace_prefix.empty())
      std::cout << namespace_prefix << ":";
    std::cout << nodename << std::endl;
  }
  else if(nodeText) //Let's say when it's text. - e.g. let's say what that white space is.
  {
    std::cout << indent << "Text Node" << std::endl;
  }

  //Treat the various node types differently:
  if(nodeText)
  {
    std::cout << indent << "text = \"" << nodeText->get_content() << "\"" << std::endl;
  }
  else if(nodeComment)
  {
    std::cout << indent << "comment = " << nodeComment->get_content() << std::endl;
  }
  else if(nodeContent)
  {
    std::cout << indent << "content = " << nodeContent->get_content() << std::endl;
  }
  else if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node))
  {
    //A normal Element node:

    //line() works only for ElementNodes.
    std::cout << indent << "     line = " << node->get_line() << std::endl;

    //Print attributes:
    const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
    for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
    {
      const xmlpp::Attribute* attribute = *iter;
      const Glib::ustring namespace_prefix = attribute->get_namespace_prefix();

      std::cout << indent << "  Attribute ";
      if(!namespace_prefix.empty())
        std::cout << namespace_prefix  << ":";
      std::cout << attribute->get_name() << " = " << attribute->get_value() << std::endl;
    }

    const xmlpp::Attribute* attribute = nodeElement->get_attribute("title");
    if(attribute)
    {
      std::cout << indent << "title = " << attribute->get_value() << std::endl;
    }
  }

  if(!nodeContent)
  {
    //Recurse through child nodes:
    xmlpp::Node::NodeList list = node->get_children();
    for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
    {
      print_node(*iter, indentation + 2); //recursive
    }
  }
}

void getPanorama(const xmlpp::Node* node, unsigned int indentation, Panorama *pano){
    const Glib::ustring indent(indentation, ' ');

    bool isDataProperties = false;
    bool isProjectionProperties = false;


    const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
    const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
    const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

    if(nodeText && nodeText->is_white_space()) //Let's ignore the indenting - you don't always want to do this.
      return;

    const Glib::ustring nodename = node->get_name();

    if(!nodeText && !nodeComment && !nodename.empty()) //Let's not say "name: text".
    {
      if(nodename=="data_properties")
        isDataProperties = true;
      if(nodename=="projection_properties")
        isProjectionProperties = true;
    }

    //Treat the various node types differently:
    if(!nodeText && !nodeComment && !nodeContent)
    {
      if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node))
        {
          //A normal Element node:

          //Print attributes:
          const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
          for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
          {
            const xmlpp::Attribute* attribute = *iter;

            if(isDataProperties){
                if(attribute->get_name()=="image_date") pano->image_date = attribute->get_value();
                if(attribute->get_name()=="pano_id") pano->pano_id = attribute->get_value();
                if(attribute->get_name()=="num_zoom_levels") pano->num_zoom_levels = atoi(attribute->get_value().c_str());
                if(attribute->get_name()=="lat") pano->lat = atof(attribute->get_value().c_str());
                if(attribute->get_name()=="lng") pano->lng = atof(attribute->get_value().c_str());

                replace(pano->pano_id," ","");
                pano->hasProjection = false;
            }

            if(isProjectionProperties){
                if(attribute->get_name()=="projection_type") pano->projection_type = attribute->get_value();
                if(attribute->get_name()=="pano_yaw_deg") pano->pano_yaw_deg = atof(attribute->get_value().c_str());
                if(attribute->get_name()=="tilt_yaw_deg") pano->tilt_yaw_deg = atof(attribute->get_value().c_str());
                if(attribute->get_name()=="tilt_pitch_deg") pano->tilt_pitch_deg = atof(attribute->get_value().c_str());
                pano->hasProjection = true;
            }
          }
        }
    }

    if(!nodeContent)
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children();
      for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
      {
        getPanorama(*iter, indentation + 2, pano); //recursive
      }
    }
}
