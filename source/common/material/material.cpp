#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 7) Write this function (DONE ✅)
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //TODO: (Req 7) Write this function (DONE ✅)
        Material::setup();
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //TODO: (Req 7) Write this function (DONE ✅)
        TintedMaterial::setup();
        shader->set("alphaThreshold", alphaThreshold);
        if (texture && sampler)
        {
            glActiveTexture(GL_TEXTURE0);
            texture->bind();
            sampler->bind(0);
            shader->set("tex", 0);
        }
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void LightingMaterial::setup() const
    {
        TintedMaterial::setup();
        //albedo
        if(albedo!=nullptr)
        {
        glActiveTexture(GL_TEXTURE0);
        albedo->bind();
        if(samplerAlbedo){
            samplerAlbedo->bind(0);
        }
        shader->set("material.albedo", 0);
        }
        //specular
        if(specular!=nullptr)
        {
        glActiveTexture(GL_TEXTURE1);
        specular->bind();
        if(samplerSpecular){
            samplerSpecular->bind(1);
        }
        shader->set("material.specular", 1);
        }
        if(emissive!=nullptr)
        {
        //emissive
        glActiveTexture(GL_TEXTURE2);
        emissive->bind();
        if(samplerEmissive){
            samplerEmissive->bind(2);
        }
        shader->set("material.emissive", 2);
        }
        if(roughness!=nullptr)
        {
        //roughness
        glActiveTexture(GL_TEXTURE3);
        roughness->bind();
        if(samplerRoughness){
            samplerRoughness->bind(3);
        }
        shader->set("material.roughness", 3);
        }
        if(ambientOcclusion!=nullptr)
        {
        //ambient_occlusion
        glActiveTexture(GL_TEXTURE4);
        ambientOcclusion->bind();
        if(samplerAmbientOcclusion){
            samplerAmbientOcclusion->bind(4);
        }
        shader->set("material.ambient_occlusion", 4);
        }
    }
    void LightingMaterial::deserialize(const nlohmann::json &data)
    { 
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        ambientOcclusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
        samplerAlbedo = AssetLoader<Sampler>::get(data.value("samplerAlbedo", ""));
        samplerSpecular = AssetLoader<Sampler>::get(data.value("samplerSpecular", ""));
        samplerRoughness = AssetLoader<Sampler>::get(data.value("samplerRoughness", ""));
        samplerAmbientOcclusion = AssetLoader<Sampler>::get(data.value("samplerAmbientOcclusion", ""));
        samplerEmissive = AssetLoader<Sampler>::get(data.value("samplerEmissive", ""));    
    }

}