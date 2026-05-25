#pragma once
#include <iostream>
#include <memory>
#include <string>

class ParticleShape;

std::unique_ptr<ParticleShape> createShape(const std::string& typeStr,
                                           std::istream& input);