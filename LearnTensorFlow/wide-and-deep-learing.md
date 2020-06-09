# Wide & Deep Learning for Recommender Systems

``` python
from __future__ import print_function
import sys
import threading
import grpc
import numpy
import tensorflow as tf
import time
from tensorflow_serving.apis import prediction_log_pb2
from tensorflow_serving.apis import predict_pb2
from tensorflow_serving.apis import prediction_service_pb2_grpc
from multiprocessing import Process
from concurrent.futures import ThreadPoolExecutor
from tensorflow.python.framework import tensor_util
from tensorflow.python.client import timeline
from tensorflow.python.profiler import model_analyzer
from tensorflow.python.profiler import option_builder


def createChannel(hostport):
    channel = grpc.insecure_channel(hostport)
    stub = prediction_service_pb2_grpc.PredictionServiceStub(channel)
    return stub

def loadReq(filePath):
    record_iterator = tf.tf_record_iterator(path=filePath)    
    reqs = []
    for string_record in record_iterator:
        p = prediction_log_pb2.PredictionLog.FromString(string_record)
        reqs.append(p.predict_log.request)
    return reqs

def predict(req, stub):
    print(stub.Predict(req, 100.0))

def cal(stub, reqs, qps):
    for x in range(0, 1):
         print("begin %d send to server" % x)
         for y in range(0, qps):
             begin = time.time()
             predict(reqs[0], stub)
             end = time.time()
             print((end - begin) * 1000)
             time.sleep(1)


if __name__ == '__main__':
    qps = int(sys.argv[2])
    stub = createChannel(sys.argv[1])   
    reqs= loadReq(sys.argv[3])
    feed_dict = {}
    filter = ['noteShareExp','noteImgLenRatio','weekClick','noteLikeRatioExp','noteLikeExp','noteShareRatioExp','devicePrice','noteClickExp','position','reqId','weekImpresion','weekEngage','noteClickRatioExp','label','noteIsVideo','noteImpressionExp','noteImgClipped','weekClickRatio','noteCollectExp','totalFollow','weekHideRatio','keywordProfileKey','authorAge','postKeywordProfileKey','weekHide','monthTotalEngage','version','noteCollectRatioExp','weekEngageRatio']
# Convert TensorProto objects to numpy
    # config = tf.ConfigProto(intra_op_parallelism_threads=1,inter_op_parallelism_threads=4)
    config = tf.ConfigProto(intra_op_parallelism_threads=1,inter_op_parallelism_threads=4)
    with tf.Session(graph=tf.Graph(),config=config) as sess:
      if len(sys.argv) > 4:
         #lib = tf.load_op_library('./sparse_embedding.so')
         graph_def = tf.GraphDef()
         with open(sys.argv[4], "rb") as f:
             graph_def.ParseFromString(f.read())
         _ = tf.import_graph_def(graph_def, name="")
         sess.run('init_all_tables')
      else:
         tf.saved_model.loader.load(sess, ["serve"], "./model")
      #g = tf.get_default_graph()
      #out = g.get_tensor_by_name("head/predictions/logistic:0")
      out = "head/predictions/logistic:0"
      #print('len of reqs: %d' % len(reqs))
      

      #for i in reqs:
      #  for j, v in i.inputs.items():
      #    if j not in filter:
      #      feed_dict[j+":0"] = tensor_util.MakeNdarray(v)
        # Warm up
        #print(feed_dict)
        #for i in range(10):
        #   outputs = sess.run([out], feed_dict=feed_dict)

        #loops = qps
        #total = 0
        #for i in range(0, loops):
        #   begin = time.time()
        #   outputs = sess.run([out], feed_dict=feed_dict)
        #   end = time.time()
        #   total += (end - begin)
        #   #print((end - begin) * 1000)
        ## print("total time: ", total)
        #print("latency: ", total / loops * 1000)
        #print("fps: ", loops / total)
        #print(outputs)

        #run_options = tf.RunOptions(trace_level=tf.RunOptions.FULL_TRACE)
        #run_metadata = tf.RunMetadata()
        #'''
        #for loop in range(8):
        #  outputs = sess.run([out], feed_dict=feed_dict,options=run_options, run_metadata=run_metadata)
        #  tl = timeline.Timeline(run_metadata.step_stats)
        #  ctf = tl.generate_chrome_trace_format()
        #  with open('timeline.json', 'w') as f:
        #    f.write(ctf)
        #'''
        #'''
        #profiler = model_analyzer.Profiler(graph=sess.graph)
        #for loop in range(1):
        #  outputs = sess.run([out], feed_dict=feed_dict,options=run_options, run_metadata=run_metadata)
        #  profiler.add_step(step=loop, run_meta=run_metadata)
        #profile_op_opt_builder = option_builder.ProfileOptionBuilder()
        #profile_op_opt_builder.select(['micros','occurrence'])
        #profile_op_opt_builder.order_by('micros')
        #profile_op_opt_builder.with_max_depth(50)
        #profiler.profile_operations(profile_op_opt_builder.build())
        #'''
        #profiler.advise(options=model_analyzer.ALL_ADVICE)
```


### Model Variables

```
len of reqs: 1
latency:  1.5615534782409668
fps:  640.3879303105672
[array([[0.31413537],
       [0.65530074],
       [0.37861967],
       [0.56650996],
       [0.11770904],
       [0.5906377 ],
       [0.2060546 ],
       [0.71547055],
       [0.67450875],
       [0.50066566],
       [0.67864776],
       [0.8155037 ],
       [0.33972025],
       [0.5946883 ],
       [0.2819834 ],
       [0.378586  ],
       [0.47518376],
       [0.45084515],
       [0.8492242 ],
       [0.21623436],
       [0.33937663],
       [0.48086697],
       [0.8560661 ],
       [0.43723333],
       [0.508817  ],
       [0.52443874],
       [0.08586571],
       [0.3393256 ],
       [0.7388899 ],
       [0.20406768],
       [0.3412019 ],
       [0.7876396 ],
       [0.6756074 ],
       [0.42793006],
       [0.7980233 ],
       [0.62154245],
       [0.48849246],
       [0.6788663 ],
       [0.46082398],
       [0.5660945 ],
       [0.60827833],
       [0.77994365],
       [0.37286744],
       [0.161398  ],
       [0.3240937 ],
       [0.19663498],
       [0.6774781 ],
       [0.06660041],
       [0.22956395],
       [0.48520443],
       [0.4700882 ],
       [0.4050689 ],
       [0.47957194],
       [0.5327469 ],
       [0.8337709 ],
       [0.8300958 ],
       [0.58005947],
       [0.70747286],
       [0.5976668 ],
       [0.4895033 ],
       [0.4078777 ],
       [0.61029935],
       [0.3764611 ],
       [0.8370978 ],
       [0.54817396],
       [0.8731864 ],
       [0.5759488 ],
       [0.38330436],
       [0.5819023 ],
       [0.53072876],
       [0.6364002 ],
       [0.01745847],
       [0.45130903],
       [0.3715834 ],
       [0.9037279 ],
       [0.80317557],
       [0.6160713 ],
       [0.53428304],
       [0.8181075 ],
       [0.7280442 ]], dtype=float32)]
---------
Variables: name (type shape) [size]
---------
dnn/input_from_feature_columns/input_layer/authorCity_viewerCity_shared_embedding/embedding_weights/part_0:0 (float32_ref 2121x32) [67872, bytes: 271488]
dnn/input_from_feature_columns/input_layer/authorGender_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 4x16) [64, bytes: 256]
dnn/input_from_feature_columns/input_layer/cityTier_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 8x16) [128, bytes: 512]
dnn/input_from_feature_columns/input_layer/loggedIn_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 3x16) [48, bytes: 192]
dnn/input_from_feature_columns/input_layer/manufacturer_embedding/embedding_weights/part_0:0 (float32_ref 693x16) [11088, bytes: 44352]
dnn/input_from_feature_columns/input_layer/networkType_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 6x16) [96, bytes: 384]
dnn/input_from_feature_columns/input_layer/noteActionIds_noteId_shared_embedding/embedding_weights/part_0:0 (float32_ref 2000000x64) [128000000, bytes: 512000000]
dnn/input_from_feature_columns/input_layer/postTaxonomyL1_taxonomyL1Profile_shared_embedding/embedding_weights/part_0:0 (float32_ref 23x16) [368, bytes: 1472]
dnn/input_from_feature_columns/input_layer/postTaxonomyL2_taxonomyL2Profile_shared_embedding/embedding_weights/part_0:0 (float32_ref 108x32) [3456, bytes: 13824]
dnn/input_from_feature_columns/input_layer/requestHour_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 26x16) [416, bytes: 1664]
dnn/input_from_feature_columns/input_layer/viewerAge_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 8x16) [128, bytes: 512]
dnn/input_from_feature_columns/input_layer/viewerGender_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 4x16) [64, bytes: 256]
dnn/input_from_feature_columns/input_layer/viewerTenure_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 5x16) [80, bytes: 320]
Total size of variables: 128083808
Total bytes of variables: 512335232

 [Begin] - SparseEmbeddingOptimizer.
 [Over] - Target node is not found.

 [Begin] - SparseEmbeddingOptimizer.
 [Over] - Target node is not found.

 [Begin] - SparseEmbeddingOptimizer.
 [Over] - Target node is not found.

 [Begin] - SparseEmbeddingOptimizer.
 [Over] - Target node is not found.

 [Begin] - SparseEmbeddingOptimizer.
    [Running] - Discovered 30 subgraphs to be merged.
 [Done] - SparseEmbeddingOptimizer.

 [Begin] - SparseEmbeddingOptimizer.
 [Over] - Target node is not found.
******** SparseEmbeddingWithShapeOp ********
******** SparseEmbeddingWithShapeOp ********
```

### Global Variables

```
len of reqs: 1
latency:  1.4690566062927246
fps:  680.7089636413505
[array([[0.31413537],
       [0.65530074],
       [0.37861967],
       [0.56650996],
       [0.11770904],
       [0.5906377 ],
       [0.2060546 ],
       [0.71547055],
       [0.67450875],
       [0.50066566],
       [0.67864776],
       [0.8155037 ],
       [0.33972025],
       [0.5946883 ],
       [0.2819834 ],
       [0.378586  ],
       [0.47518376],
       [0.45084515],
       [0.8492242 ],
       [0.21623436],
       [0.33937663],
       [0.48086697],
       [0.8560661 ],
       [0.43723333],
       [0.508817  ],
       [0.52443874],
       [0.08586571],
       [0.3393256 ],
       [0.7388899 ],
       [0.20406768],
       [0.3412019 ],
       [0.7876396 ],
       [0.6756074 ],
       [0.42793006],
       [0.7980233 ],
       [0.62154245],
       [0.48849246],
       [0.6788663 ],
       [0.46082398],
       [0.5660945 ],
       [0.60827833],
       [0.77994365],
       [0.37286744],
       [0.161398  ],
       [0.3240937 ],
       [0.19663498],
       [0.6774781 ],
       [0.06660041],
       [0.22956395],
       [0.48520443],
       [0.4700882 ],
       [0.4050689 ],
       [0.47957194],
       [0.5327469 ],
       [0.8337709 ],
       [0.8300958 ],
       [0.58005947],
       [0.70747286],
       [0.5976668 ],
       [0.4895033 ],
       [0.4078777 ],
       [0.61029935],
       [0.3764611 ],
       [0.8370978 ],
       [0.54817396],
       [0.8731864 ],
       [0.5759488 ],
       [0.38330436],
       [0.5819023 ],
       [0.53072876],
       [0.6364002 ],
       [0.01745847],
       [0.45130903],
       [0.3715834 ],
       [0.9037279 ],
       [0.80317557],
       [0.6160713 ],
       [0.53428304],
       [0.8181075 ],
       [0.7280442 ]], dtype=float32)]
---------
Variables: name (type shape) [size]
---------
global_step:0 (int64_ref ) [1, bytes: 8]
dnn/input_from_feature_columns/input_layer/authorCity_viewerCity_shared_embedding/embedding_weights/part_0:0 (float32_ref 2121x32) [67872, bytes: 271488]
dnn/input_from_feature_columns/input_layer/authorGender_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 4x16) [64, bytes: 256]
dnn/input_from_feature_columns/input_layer/cityTier_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 8x16) [128, bytes: 512]
dnn/input_from_feature_columns/input_layer/loggedIn_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 3x16) [48, bytes: 192]
dnn/input_from_feature_columns/input_layer/manufacturer_embedding/embedding_weights/part_0:0 (float32_ref 693x16) [11088, bytes: 44352]
dnn/input_from_feature_columns/input_layer/networkType_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 6x16) [96, bytes: 384]
dnn/input_from_feature_columns/input_layer/noteActionIds_noteId_shared_embedding/embedding_weights/part_0:0 (float32_ref 2000000x64) [128000000, bytes: 512000000]
dnn/input_from_feature_columns/input_layer/postTaxonomyL1_taxonomyL1Profile_shared_embedding/embedding_weights/part_0:0 (float32_ref 23x16) [368, bytes: 1472]
dnn/input_from_feature_columns/input_layer/postTaxonomyL2_taxonomyL2Profile_shared_embedding/embedding_weights/part_0:0 (float32_ref 108x32) [3456, bytes: 13824]
dnn/input_from_feature_columns/input_layer/requestHour_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 26x16) [416, bytes: 1664]
dnn/input_from_feature_columns/input_layer/viewerAge_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 8x16) [128, bytes: 512]
dnn/input_from_feature_columns/input_layer/viewerGender_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 4x16) [64, bytes: 256]
dnn/input_from_feature_columns/input_layer/viewerTenure_bucketized_embedding/embedding_weights/part_0:0 (float32_ref 5x16) [80, bytes: 320]
dnn/hiddenlayer_0/kernel/part_0:0 (float32 432x256) [110592, bytes: 442368]
dnn/hiddenlayer_0/bias/part_0:0 (float32 256) [256, bytes: 1024]
dnn/hiddenlayer_1/kernel/part_0:0 (float32 256x128) [32768, bytes: 131072]
dnn/hiddenlayer_1/bias/part_0:0 (float32 128) [128, bytes: 512]
dnn/hiddenlayer_2/kernel/part_0:0 (float32 128x64) [8192, bytes: 32768]
dnn/hiddenlayer_2/bias/part_0:0 (float32 64) [64, bytes: 256]
dnn/logits/kernel/part_0:0 (float32 64x1) [64, bytes: 256]
dnn/logits/bias/part_0:0 (float32 1) [1, bytes: 4]
linear/linear_model/nodes/weights/part_0:0 (float32 9600x1) [9600, bytes: 38400]
linear/linear_model/postTaxonomyL1/weights/part_0:0 (float32 23x1) [23, bytes: 92]
linear/linear_model/postTaxonomyL1_X_taxonomyL1Profile/weights/part_0:0 (float32 529x1) [529, bytes: 2116]
linear/linear_model/postTaxonomyL2/weights/part_0:0 (float32 108x1) [108, bytes: 432]
linear/linear_model/postTaxonomyL2_X_taxonomyL2Profile/weights/part_0:0 (float32 11664x1) [11664, bytes: 46656]
linear/linear_model/taxonomyL1Profile/weights/part_0:0 (float32 23x1) [23, bytes: 92]
linear/linear_model/taxonomyL2Profile/weights/part_0:0 (float32 108x1) [108, bytes: 432]
linear/linear_model/viewerAge_bucketized/weights/part_0:0 (float32 8x1) [8, bytes: 32]
linear/linear_model/viewerAge_bucketized_X_viewerGender_bucketized/weights/part_0:0 (float32 30x1) [30, bytes: 120]
linear/linear_model/viewerAge_bucketized_X_viewerGender_bucketized_X_viewerTenure_bucketized/weights/part_0:0 (float32 120x1) [120, bytes: 480]
linear/linear_model/viewerGender_bucketized/weights/part_0:0 (float32 4x1) [4, bytes: 16]
linear/linear_model/viewerGender_bucketized_X_viewerTenure_bucketized/weights/part_0:0 (float32 12x1) [12, bytes: 48]
linear/linear_model/viewerTenure_bucketized/weights/part_0:0 (float32 5x1) [5, bytes: 20]
linear/linear_model/bias_weights/part_0:0 (float32 1) [1, bytes: 4]
Total size of variables: 128258109
Total bytes of variables: 513032440

 [Begin] - SparseEmbeddingOptimizer.
 [Over] - Target node is not found.

 [Begin] - SparseEmbeddingOptimizer.
 [Over] - Target node is not found.

 [Begin] - SparseEmbeddingOptimizer.
 [Over] - Target node is not found.

 [Begin] - SparseEmbeddingOptimizer.
 [Over] - Target node is not found.

 [Begin] - SparseEmbeddingOptimizer.
    [Running] - Discovered 30 subgraphs to be merged.
 [Done] - SparseEmbeddingOptimizer.

 [Begin] - SparseEmbeddingOptimizer.
 [Over] - Target node is not found.
******** SparseEmbeddingWithShapeOp ********
******** SparseEmbeddingWithShapeOp ********
```
