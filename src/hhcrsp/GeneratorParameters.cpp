#include "GeneratorParameters.h"

#include <tuple>

using namespace std;

GeneratorParameters::GeneratorParameters(std::mt19937 &rng, LocationGeneratorI &locGen):
   m_rng(rng), m_locGen(locGen) {

   m_numDays = 1;
   m_numCaregivers = 20;
   m_numDailyPatients = 100;
   m_numSkills = 6;
}

GeneratorParameters::~GeneratorParameters() {
   // Empty
}

std::mt19937 &GeneratorParameters::prng() {
   return m_rng;
}

LocationGeneratorI &GeneratorParameters::locationGenerator() {
   return m_locGen;
}

int GeneratorParameters::numDays() const {
   return m_numDays;
}

int GeneratorParameters::numCaregivers() const {
   return m_numCaregivers;
}

int GeneratorParameters::numDailyPatients() const {
   return m_numDailyPatients;
}

int GeneratorParameters::numSkills() const {
   return m_numSkills;
}

int GeneratorParameters::poolSizeOverride() const {
   return 100;
}

double GeneratorParameters::simultaneousDoubleServicesPerc() const {
   return 0.15;
}

double GeneratorParameters::precedenceDoubleServicesPerc() const {
   return 0.15;
}

int GeneratorParameters::maxSkillsPerCaregiver() const {
   return 3;
}

int GeneratorParameters::minPatientsPerSkillUnit() const {
   return 4;
}

int GeneratorParameters::maxPatientsPerSkillUnit() const {
   return 6;
}

int GeneratorParameters::dayStartingMinutes() const {
   return 8*60;
}

int GeneratorParameters::dayEndingMinutes() const {
   return 19*60;
}

int GeneratorParameters::twDurationMinutes() const {
   return 2*60;
}

int GeneratorParameters::minProcTime() const {
   return 15;
}

int GeneratorParameters::maxProcTime() const {
   return 80;
}

int GeneratorParameters::minSeparation() const {
   return 20;
}

int GeneratorParameters::maxSeparation() const {
   return 60;
}
