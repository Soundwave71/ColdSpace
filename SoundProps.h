//
// Created by Leonardo on 7/8/2017.
//

#ifndef COLDSPACE_SOUNDPROPS_H
#define COLDSPACE_SOUNDPROPS_H
struct SoundProps{
    SoundProps(const std::string& l_name): m_audioName(l_name),m_volume(100), m_pitch(1.f),
                                           m_minDistance(10.f), m_attenuation(10.f){}
    std::string m_audioName;
    float m_volume;
    float m_pitch;
    float m_minDistance;
    float m_attenuation;
};
#endif //COLDSPACE_SOUNDPROPS_H
