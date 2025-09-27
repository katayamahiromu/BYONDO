#pragma once

#include <memory>
#define XAUDIO2_HELPER_FUNCTIONS
#include <xaudio2.h>
#include "Audio/AudioResource.h"

// オーディオソース
class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
	~AudioSource();

	// 再生
	void Play(bool loop);

	//呼ばれるとまた最初から再生
	void DC_Play();

	// 停止
	void Stop();

	//ボリューム
	void SetVolume(float volum);

	//デシベルを設定
	float SetDecibel(float decibls);

	//ピッチ
	void SetPitch(float Pitch);

	//一時停止
	void Pause();

	//リスタート
	void ReStart();

	//フィルター
	void Filter(int type);

	//再生中か
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
