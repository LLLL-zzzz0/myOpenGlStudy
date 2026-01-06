#pragma once

#include "../core.h"

enum class MaterialType
{
    PhongMaterial,
    WhiteMaterial
};

class Material 
{
public:
    Material();
    virtual ~Material();
    MaterialType getMaterialType();

protected:
    MaterialType m_enumType{ MaterialType::PhongMaterial };
};