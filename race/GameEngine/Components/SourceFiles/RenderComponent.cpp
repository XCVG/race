#include "RenderComponent.h"
void RenderComponent::setModelName(std::string _name)
{
    this->_modelName = _name;
};
void RenderComponent::setAlbedoName(std::string _name)
{
    this->_albedoName = _name;
};
void RenderComponent::setNormalName(std::string _name)
{
    this->_normalName = _name;
};
void RenderComponent::setSmoothness(GLfloat _value)
{
    this->_smoothness = _value;
};
std::string RenderComponent::getModelName()
{
    return this->_modelName;
};
std::string RenderComponent::getAlbedoName()
{
    return this->_albedoName;
};
std::string RenderComponent::getNormalName()
{
    return this->_normalName;
};
GLfloat RenderComponent::getSmoothness() 
{
    return this->_smoothness;
};