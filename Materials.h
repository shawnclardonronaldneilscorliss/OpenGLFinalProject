#ifndef _MATERIALS
#define _MATERIALS

#include <iostream>
#include <array>

//setup the background color of the scene
float background_Color[] = { 1.0f, 1.0f, 1.0f, 0.0f };

//define the material choices used to color the objects
//Material: Red Plastic
float redPlastic_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float redPlastic_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
float redPlastic_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
float redPlasticshine = 32.0f*128.0f;

//Material: Red seethrough plastic
float redPlasticAlpha_ambient[] = { 0.0f, 0.0f, 0.0f, 0.5f };
float redPlasticAlpha_diffuse[] = { 0.5f, 0.0f, 0.0f, 0.5f };
float redPlasticAlpha_specular[] = { 0.7f, 0.6f, 0.6f, 0.5f };
float redPlasticAlphashine = 32.0f*128.0f;

//Material: Gold
float gold_ambient[] = { 0.24725f, 0.1995f, 0.0745f, 1.0f };
float gold_diffuse[] = { 0.75164f, 0.60648f, 0.22648f, 1.0f };
float gold_specular[] = { 0.628281f, 0.555802f, 0.366065f, 1.0f };
float goldshine = 51.2f*128.0f;

//Material: Emerald
float emerald_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 0.55f };
float emerald_diffuse[] = { 0.07568f, 0.61424f, 0.07568f, 0.55f };
float emerald_specular[] = { 0.633f, 0.727811f, 0.633f, 0.55f };
float emeraldshine = 30.8f*128.0f;

//Material:grass
float grass_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 1.f };
float grass_diffuse[] = { 0.07568f, 0.61424f, 0.07568f, 1.0f };
float grass_specular[] = { 0.633f, 0.727811f, 0.633f, 1.0f };
float grassshine = 30.8f*128.0f;

//Material: jade
float jade_ambient[] = { 0.135f, 0.2225f, 0.1575f, 0.95f };
float jade_diffuse[] = { 0.54f, 0.89f, 0.63f, 0.95f };
float jade_specular[] = { 0.316228f, 0.316228f, 0.316228f, 0.95f };
float jadeshine = 12.8f*128.0f;

//Material: Polished silver
float silver_ambient[] = { 0.23125f, 0.23125f, 0.23125f, 1.0f };
float silver_diffuse[] = { 0.2775f, 0.2775f, 0.2775f, 1.0f };
float silver_specular[] = { 0.773911f, 0.773911f, 0.773911f, 1.0f };
float silvershine = 89.6f*128.0f;

//Material: white plastic
float whiteplastic_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
float whiteplastic_diffuse[] = { 0.55f, 0.55f, 0.55f, 1.0f };
float whiteplastic_specular[] = { 0.70f, 0.70f, 0.70f, 1.0f };
float whiteplasticshine = 32.0f*128.0f;

//Material: Gray Plastic
float greyplastic_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float greyplastic_diffuse[] = { 0.55f, 0.55f, 0.55f, 1.0f };
float greyplastic_specular[] = { 0.70f, 0.70f, 0.70f, 1.0f };
float greyplasticshine = 32.0f*128.0f;

//Material: clear glass
float clearGlass_ambient[] = { 0.0f, 0.0f, .2f, 0.523f };
float clearGlass_diffuse[] = { 0.0f, 0.0f, .2f, 0.53f };
float clearGlass_specular[] = { 0.0f, 0.0f, 1.0f, 0.23f };
float clearGlassshine = 90.8f*128.0f;

//Material: Brown
float brown_ambient[] = { 0.5f, 0.35f, 0.05f,1.0f };
float brown_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
float brown_specular[] = { 0.5f, 0.35f, 0.05f, 1.0f };
float brownshine = 25.0f*128.0f;

//Material: Concrete
float concrete_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
float concrete_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
float concrete_specular[] = { 0.70f, 0.70f, 0.70f, 1.0f };
float concreteshine = 1.0f*128.0f;

//Material: black rubber
float blackrubber_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
float blackrubber_diffuse[] = { 0.01f, 0.01f, 0.01f, 1.0f };
float blackrubber_specular[] = { 0.40f, 0.40f, 0.40f, 1.0f };
float blackrubbershine = .078125f * 128.0f;

//Material: yellow rubber
float yellowplastic_ambient[] = { 0.00f, 0.00f, 0.0f, 1.0f };
float yellowplastic_diffuse[] = { 0.5f, 0.5f, 0.0f, 1.0f };
float yellowplastic_specular[] = { 0.6f, 0.6f, 0.05f, 1.0f };
float yellowplasticshine = .50f * 128.0f;

#endif // !_MATERIALS.H
