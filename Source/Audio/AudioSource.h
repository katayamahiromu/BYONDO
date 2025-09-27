#pragma once

#include <memory>
#define XAUDIO2_HELPER_FUNCTIONS
#include <xaudio2.h>
#include "Audio/AudioResource.h"

// �I�[�f�B�I�\�[�X
class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
	~AudioSource();

	// �Đ�
	void Play(bool loop);

	//�Ă΂��Ƃ܂��ŏ�����Đ�
	void DC_Play();

	// ��~
	void Stop();

	//�{�����[��
	void SetVolume(float volum);

	//�f�V�x����ݒ�
	float SetDecibel(float decibls);

	//�s�b�`
	void SetPitch(float Pitch);

	//�ꎞ��~
	void Pause();

	//���X�^�[�g
	void ReStart();

	//�t�B���^�[
	void Filter(int type);

	//�Đ�����
	bool IsPlay();
private:
	IXAudio2SourceVoice*			sourceVoice = nullptr;
	std::shared_ptr<AudioResource>	resource;
	float volum_ = 1.0;
public:
	enum Fliter_Type
	{
		LOW_PASS_FILTER,
		HIGH_PASS_FILTER,
		BAND_PASS_FILTER,
		NOTCH_FILTER,
		LOW_PASS_ONE_POLE_FILTER,
		HIGH_PASS_ONE_POLE_FILTER,
	};
};
