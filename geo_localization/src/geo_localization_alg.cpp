#include "geo_localization_alg.h"

GeoLocalizationAlgorithm::GeoLocalizationAlgorithm(void)
{
  pthread_mutex_init(&this->access_,NULL);
}

GeoLocalizationAlgorithm::~GeoLocalizationAlgorithm(void)
{
  pthread_mutex_destroy(&this->access_);
}

void GeoLocalizationAlgorithm::config_update(Config& config, uint32_t level)
{
  this->lock();

  // save the current configuration
  this->config_=config;
  
  this->unlock();
}

// GeoLocalizationAlgorithm Public API
