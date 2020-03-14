

#ifndef TRT_BUILDER_HPP
#define TRT_BUILDER_HPP

#include <string>
#include <vector>
#include <functional>
#include <opencv2/opencv.hpp>

namespace TRTBuilder {

	//INT8ģʽ�µ��Զ���Ԥ������
	typedef std::function<void(int current, int count, cv::Mat& inputOutput)> Int8Process;

	//cudaSetDevice�ķ�װ���������cudaͷ�ļ�
	void setDevice(int device_id);

	enum ModelSourceType {
		//����������uffģ�͵�֧�֣������Լ�д��ȥ
		ModelSourceType_FromCaffe,
		ModelSourceType_FromONNX
	};

	//����ģ�͵���Դ��ʽ��ͨ������࣬�Զ��ķֱ�������caffe����onnx�����ڴ���
	class ModelSource {
	public:
		ModelSource(const std::string& prototxt, const std::string& caffemodel);
		ModelSource(const std::string& onnxmodel);
		ModelSourceType type() const;
		std::string prototxt() const;
		std::string caffemodel() const;
		std::string onnxmodel() const;

	private:
		std::string prototxt_, caffemodel_;
		std::string onnxmodel_;
		ModelSourceType type_;
	};

	enum TRTMode {
		TRTMode_FP32,
		TRTMode_FP16,
		TRTMode_INT8
	};

	//��ģʽת��Ϊ�ַ���FP32��FP16��INT8
	const char* modeString(TRTMode type);


	//������INT8ģʽʱ��int8process�����ƶ�
	//     int8ImageDirectory��int8EntropyCalibratorFileָ��һ������
	//     ����������ɣ�ָ����int8EntropyCalibratorFile��calibrator�ᱣ�浽int8EntropyCalibratorFileָ�����ļ�
	//     ����Ѿ����ɹ���ָ����int8EntropyCalibratorFile��calibrator���int8EntropyCalibratorFileָ�����ļ����أ�������
	//          ��int8ImageDirectory��ȡͼƬ����������
	//������FP32����FP16ʱ��int8process��int8ImageDirectory��int8EntropyCalibratorFile������Ҫָ��
	bool compileTRT(
		TRTMode mode,
		const std::vector<std::string>& outputs,
		unsigned int maxBatchSize,
		const ModelSource& source,
		const std::string& savepath,
		Int8Process int8process = nullptr,					
		const std::string& int8ImageDirectory = "",
		const std::string& int8EntropyCalibratorFile = "");
};

#endif //TRT_BUILDER_HPP