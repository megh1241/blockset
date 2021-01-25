import io
import boto3
import struct 

#### GLOBALS ####

size_of_node = 16
node_type_format = 'iiif'
bucketname = 'your bucket name'


def readMetadata(f):
    global num_classes
    global num_bins
    global bin_sizes
    global bin_node_sizes
    global bin_start_arr

    bin_sizes = []
    bin_node_sizes = []
    bin_start_arr = []

    data = f.read(-1)
    data_list = data.split(b'\n')
    data_list_final = [int(float(i)) for i in data_list if i!= b'']
    num_classes = data_list_final[0]
    num_bins = data_list_final[1]
    k = 2

    for i in range(num_bins):
        bin_sizes.append(data_list_final[k])
        k+=1

    for i in range(num_bins):
        bin_node_sizes.append(data_list_final[k])
        k+=1
        
    for i in range(num_bins):
        bin_start = []
        for j in range(bin_sizes[i]):
            bin_start.append(data_list_final[k])
            k+=1
        bin_start_arr.append(bin_start)



def upload_file(file_name, bucket, object_name=None):
    """Upload a file to an S3 bucket

    :param file_name: File to upload
    :param bucket: Bucket to upload to
    :param object_name: S3 object name. If not specified then file_name is used
    :return: True if file was uploaded, else False
    """

    # If S3 object_name was not specified, use file_name
    if object_name is None:
        object_name = file_name

    # Upload the file
    s3_client = boto3.client('s3')
    try:
        response = s3_client.upload_file(file_name, bucket, object_name)
    except ClientError as e:
        logging.error(e)
        return False
    return True


def readObsArrFromS3(s3_file):
    X = s3_file.read(-1)
    X_new = X.split(b'\n')

    obs = []
    for row  in X_new:
        elements = row.split(b',')
        new_elements = [float(i) for i in elements if i!= b'']
        obs.append(new_elements)

    return obs


def getNodeFromBucket(f, node_index):
    """
    Read just the node from the s3 bucket.
    Convert bytes to node format.
    :returns node
    """
    #Beginning of node
    start_ptr = size_of_node * node_index
    
    #Seek to node 
    f.seek_beg()
    f.seek(start_ptr)

    #Read size_of_node bytes
    node_bytes = f.read(size_of_node)

    #Convert bytes to node format of ints and float
    node = struct.unpack(node_type_format, node_bytes)

    #return left, right, feature, threshold 
    return node[0], node[1], node[2], node[3]


def predict(X, bucket_file_handle):
    """
    Predicts label for a test observation
    one at a time
    :param redis client and X
    :returns predicted label
    """

    num_trees = bin_sizes[0]
    preds = [0] * num_classes
    curr_node = [i for i in range(num_trees)]

    while True:
        number_not_in_leaf = 0
        for i in range(num_trees):
            if curr_node[i] >= 0:
                left, right, feature, threshold =\
                        getNodeFromBucket(bucket_file_handle, curr_node[i])
                if left > -1:
                    obs_feature_val = X[feature]
                    if obs_feature_val <= threshold:
                        curr_node[i] = left
                    else:
                        curr_node[i] = right
                    number_not_in_leaf += 1

        if number_not_in_leaf <= 0:
            break

    for i in range(num_trees):
        class_label = getNodeFromBucket(bucket_file_handle, curr_node[i] )[1]
        preds[class_label] += 1
    
    return preds.index(max(preds))


def validatePredictions(predicted, y):
    right = 0
    total = 0
    for i, j in zip(predicted, y):
        if i==j:
            right+=1
        total+=1

    return float(right) / float(total)


class S3File(io.RawIOBase):
    def __init__(self, s3_object):
        self.s3_object = s3_object
        self.position = 0

    def __repr__(self):
        return "<%s s3_object=%r>" % (type(self).__name__, self.s3_object)

    @property
    def size(self):
        return self.s3_object.content_length

    def tell(self):
        return self.position

    def seek_beg(self):
        self.position = 0

    def seek(self, offset, whence=io.SEEK_SET):
        if whence == io.SEEK_SET:
            self.position = offset
        elif whence == io.SEEK_CUR:
            self.position += offset
        elif whence == io.SEEK_END:
            self.position = self.size + offset
        else:
            raise ValueError("invalid whence (%r, should be %d, %d, %d)" % (
                whence, io.SEEK_SET, io.SEEK_CUR, io.SEEK_END
            ))

        return self.position

    def seekable(self):
        return True

    def read(self, size=-1):
        if size == -1:
            # Read to the end of the file
            range_header = "bytes=%d-" % self.position
            self.seek(offset=0, whence=io.SEEK_END)
        else:
            new_position = self.position + size

            # If we're going to read beyond the end of the object, return
            # the entire object.
            if new_position >= self.size:
                return self.read()

            range_header = "bytes=%d-%d" % (self.position, new_position - 1)
            self.seek(offset=size, whence=io.SEEK_CUR)

        return self.s3_object.get(Range=range_header)["Body"].read()

    def readable(self):
        return True


    
s3 = boto3.resource("s3")
upload_file(file_name = "iris.csv", bucket = bucketname )
s3_object_X = s3.Object(bucket_name=bucketname, key="iris.csv")
s3_object_meta = s3.Object(bucket_name=bucketname, key="metadata.txt")
s3_object_model = s3.Object(bucket_name=bucketname, key="packedmodel.bin")


s3_file_meta = S3File(s3_object_meta)
s3_file_model = S3File(s3_object_model)
s3_file_X = S3File(s3_object_X)

readMetadata(s3_file_meta)
observations = readObsArrFromS3(s3_file_X)

for num, obs in enumerate(observations):
    if  len(obs) <= 0:
        continue
    print(obs)
