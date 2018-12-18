#include <iostream>
#include <vector>
#include <chrono>

#include "mkldnn.hpp"


void simple_net()
{
  /*
   * Initialize the engine
   */
  auto cpu_engine = mkldnn::engine(mkldnn::engine::cpu, 0);

  const uint32_t batch = 256;
  std::vector<float> net_src(batch * 3 * 227 * 227);
  std::vector<float> net_dst(batch * 96 * 27 * 27);

  /* AlexNet: conv
   * {batch, 3, 227, 227} (x) {96, 3, 11, 11} -> {batch, 96, 55, 55}
   * strides:{4, 4}
   */
  mkldnn::memory::dims conv_src_tz = {batch, 3, 227, 227};
  mkldnn::memory::dims conv_weights_tz = {96, 3, 11, 11};
  mkldnn::memory::dims conv_bias_tz = {96};
  mkldnn::memory::dims conv_dst_tz = {batch, 96, 55, 55};
  mkldnn::memory::dims conv_strides = {4, 4};
  auto conv_padding = {0, 0};
       
  std::vector<float> conv_weights(std::accumulate(conv_weights_tz.begin(), 
              conv_weights_tz.end(), 1, std::multiplies<uint32_t>()));

  std::vector<float> conv_bias(std::accumulate(conv_bias_tz.begin(),
              conv_bias_tz.end(), 1, std::multiplies<uint32_t>()));

  /*
   * Create memory for user data
   */
  auto conv_user_src_memory = mkldnn::memory({{{conv_src_tz}, 
      mkldnn::memory::data_type::f32, 
      mkldnn::memory::format::nchw}, cpu_engine}, net_src.data());

  auto conv_user_weights_memory = mkldnn::memory({{{conv_weights_tz}, 
      mkldnn::memory::data_type::f32, 
      mkldnn::memory::format::oihw}, cpu_engine}, conv_weights.data());

  auto conv_user_bias_memory = mkldnn::memory({{{conv_bias_tz}, 
      mkldnn::memory::data_type::f32, 
      mkldnn::memory::format::x}, cpu_engine}, conv_bias.data());

  /*
   * Create some descriptors
   */
  auto conv_src_md = mkldnn::memory::desc({conv_src_tz}, 
      mkldnn::memory::data_type::f32,
      mkldnn::memory::format::any);

  auto conv_bias_md = mkldnn::memory::desc({conv_bias_tz},
      mkldnn::memory::data_type::f32,
      mkldnn::memory::format::any);

  auto conv_weights_md = mkldnn::memory::desc({conv_weights_tz},
      mkldnn::memory::data_type::f32, 
      mkldnn::memory::format::any);

  auto conv_dst_md = mkldnn::memory::desc({conv_dst_tz}, 
      mkldnn::memory::data_type::f32, 
      mkldnn::memory::format::any);

  auto conv_desc = mkldnn::convolution_forward::desc(mkldnn::prop_kind::forward,
      mkldnn::convolution_direct, conv_src_md, conv_weights_md, conv_bias_md, 
      conv_dst_md, conv_strides, conv_padding, conv_padding, 
      mkldnn::padding_kind::zero);

  auto conv_prim_desc = mkldnn::convolution_forward::primitive_desc(conv_desc, cpu_engine);

  /*
   * Create Net
   */
  std::vector<mkldnn::primitive> net;

  auto conv_src_memory = conv_user_src_memory;
  if (mkldnn::memory::primitive_desc(conv_prim_desc.src_primitive_desc()) != 
      conv_user_src_memory.get_primitive_desc()) 
   {
    conv_src_memory = mkldnn::memory(conv_prim_desc.src_primitive_desc());
    net.push_back(mkldnn::reorder(conv_user_src_memory, conv_src_memory));
   }
     
  auto conv_weights_memory = conv_user_weights_memory;
  if (mkldnn::memory::primitive_desc(conv_prim_desc.weights_primitive_desc()) != 
      conv_user_weights_memory.get_primitive_desc()) 
   {

    conv_weights_memory = mkldnn::memory(conv_prim_desc.weights_primitive_desc());
    net.push_back(mkldnn::reorder(conv_user_weights_memory, conv_weights_memory));
   }
     
  auto conv_dst_memory = mkldnn::memory(conv_prim_desc.dst_primitive_desc());

  net.push_back(mkldnn::convolution_forward(conv_prim_desc, conv_src_memory, 
        conv_weights_memory, conv_user_bias_memory, conv_dst_memory));

  /* AlexNet: relu
   * {batch, 96, 55, 55} -> {batch, 96, 55, 55}
   */
  const double negative_slope = 1.0;

  // original relu
  //auto relu_dst_memory = mkldnn::memory(conv_prim_desc.dst_primitive_desc());

  //auto relu_desc = mkldnn::relu_forward::desc(mkldnn::prop_kind::forward, 
  //    conv_prim_desc.dst_primitive_desc().desc(), negative_slope);

  //auto relu_prim_desc = mkldnn::relu_forward::primitive_desc(relu_desc, cpu_engine);

  //net.push_back(mkldnn::relu_forward(relu_prim_desc, conv_dst_memory, relu_dst_memory));

  // Eltwise alternate
  auto relu_desc = mkldnn::eltwise_forward::desc(mkldnn::prop_kind::forward,
      mkldnn::algorithm::eltwise_relu,
      conv_prim_desc.dst_primitive_desc().desc(), negative_slope);

  auto relu_prim_desc = mkldnn::eltwise_forward::primitive_desc(relu_desc, cpu_engine);

  auto relu_dst_memory = mkldnn::memory(relu_prim_desc.dst_primitive_desc());

  auto relu_fd = mkldnn::eltwise_forward(relu_prim_desc, conv_dst_memory, relu_dst_memory);
  net.push_back(relu_fd);

  /* AlexNet: lrn
   * {batch, 96, 55, 55} -> {batch, 96, 55, 55}
   * local size:5
   * alpha:0.0001
   * beta:0.75
   */
  const uint32_t local_size = 5;
  const double alpha = 0.0001;
  const double beta = 0.75;

  auto lrn_dst_memory = mkldnn::memory(relu_dst_memory.get_primitive_desc());
  /* create lrn scratch memory from lrn src */
  auto lrn_scratch_memory = mkldnn::memory(lrn_dst_memory.get_primitive_desc());
  
  /* create lrn primitive and add it to net */
  auto lrn_desc = mkldnn::lrn_forward::desc(mkldnn::prop_kind::forward, mkldnn::lrn_across_channels, 
      conv_prim_desc.dst_primitive_desc().desc(), local_size, alpha, beta);

  auto lrn_prim_desc = mkldnn::lrn_forward::primitive_desc(lrn_desc, cpu_engine);

  net.push_back(mkldnn::lrn_forward(lrn_prim_desc, relu_dst_memory, lrn_scratch_memory, lrn_dst_memory));

  /* AlexNet: pool
   * {batch, 96, 55, 55} -> {batch, 96, 27, 27}
   * kernel:{3, 3}
   * strides:{2, 2}
   */
  mkldnn::memory::dims pool_dst_tz = {batch, 96, 27, 27};
  mkldnn::memory::dims pool_kernel = {3, 3};
  mkldnn::memory::dims pool_strides = {2, 2};
  auto pool_padding = {0, 0};

  auto pool_user_dst_memory = mkldnn::memory({{{pool_dst_tz}, 
      mkldnn::memory::data_type::f32, 
      mkldnn::memory::format::nchw}, cpu_engine}, net_dst.data());

  auto pool_dst_md = mkldnn::memory::desc({pool_dst_tz}, 
      mkldnn::memory::data_type::f32, 
      mkldnn::memory::format::any);

  auto pool_desc = mkldnn::pooling_forward::desc(mkldnn::prop_kind::forward, 
      mkldnn::pooling_max, lrn_dst_memory.get_primitive_desc().desc(), 
      pool_dst_md, pool_strides, pool_kernel, pool_padding, pool_padding,mkldnn::padding_kind::zero);

  auto pool_pd = mkldnn::pooling_forward::primitive_desc(pool_desc, cpu_engine);

  auto pool_dst_memory = pool_user_dst_memory;
  if (mkldnn::memory::primitive_desc(pool_pd.dst_primitive_desc()) != 
      pool_user_dst_memory.get_primitive_desc()) 
   {
     pool_dst_memory = mkldnn::memory(pool_pd.dst_primitive_desc());
   }

  auto pool_indices_memory = mkldnn::memory(pool_dst_memory.get_primitive_desc());

  net.push_back(mkldnn::pooling_forward(pool_pd, lrn_dst_memory, pool_indices_memory, pool_dst_memory));

  if (pool_dst_memory != pool_user_dst_memory) 
   {
     net.push_back(mkldnn::reorder(pool_dst_memory, pool_user_dst_memory));
   }

  /*
   * Create stream
   */
  auto start = std::chrono::system_clock::now();
  mkldnn::stream(mkldnn::stream::kind::eager).submit(net).wait();
  auto end = std::chrono::system_clock::now();
  std::cout << "Inference Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << " ms" << std::endl;
  
}


int main(int argc, char** argv)
{

  try {
    simple_net();
  }
  catch (mkldnn::error &e) {
    std::cerr << "status: " << e.status << std::endl;
    std::cerr << "message: " << e.message << std::endl;
  }

  return 0;
}
