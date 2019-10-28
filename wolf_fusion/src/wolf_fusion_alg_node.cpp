#include "wolf_fusion_alg_node.h"

WolfFusionAlgNode::WolfFusionAlgNode(void) :
  algorithm_base::IriBaseAlgorithm<WolfFusionAlgorithm>()
{
  //init class attributes if necessary
  //this->loop_rate_ = 2;//in [Hz]

  // [init publishers]
  
  // [init subscribers]
  
  // [init services]
  
  // [init clients]
  
  // [init action servers]
  
  // [init action clients]
}

WolfFusionAlgNode::~WolfFusionAlgNode(void)
{
  // [free dynamic memory]
}

void WolfFusionAlgNode::mainNodeThread(void)
{
  // [fill msg structures]
  
  // [fill srv structure and make request to the server]
  
  // [fill action structure and make request to the action server]

  // [publish messages]
}

/*  [subscriber callbacks] */

/*  [service callbacks] */

/*  [action callbacks] */

/*  [action requests] */

void WolfFusionAlgNode::node_config_update(Config &config, uint32_t level)
{
  this->alg_.lock();
  this->config_=config;
  this->alg_.unlock();
}

void WolfFusionAlgNode::addNodeDiagnostics(void)
{
}

/* main function */
int main(int argc,char *argv[])
{
  return algorithm_base::main<WolfFusionAlgNode>(argc, argv, "wolf_fusion_alg_node");
}