/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#include "Pch.hpp"

// Forward-declare stb_vorbis types so miniaudio can see them.
// Full implementation is compiled separately in StbVorbis.c (as C).
#define STB_VORBIS_HEADER_ONLY
extern "C" {
#include "extras/stb_vorbis.c"
}
#undef STB_VORBIS_HEADER_ONLY

#define MINIAUDIO_IMPLEMENTATION
#include "Audio.hpp"

namespace def
{
	AudioHandler::AudioHandler()
	{
		if (ma_engine_init(nullptr, &m_SoundEngine) != MA_SUCCESS)
			std::cerr << "[Audio] Failed to initialise audio engine" << std::endl;
	}

	AudioHandler::~AudioHandler()
	{
		m_Sounds.clear();
		ma_engine_uninit(&m_SoundEngine);
	}

	bool AudioHandler::Load(uint32_t id, std::string_view path)
	{
		auto it = m_Sounds.find(id);

		if (it != m_Sounds.end())
		{
			ma_sound_uninit(it->second.get());
			m_Sounds.erase(it);
		}

		auto sound = std::make_unique<ma_sound>();

		ma_result result = ma_sound_init_from_file(
			&m_SoundEngine,
			std::string(path).c_str(),
			MA_SOUND_FLAG_DECODE,
			nullptr, nullptr,
			sound.get()
		);

		if (result != MA_SUCCESS)
		{
			std::cerr << "[Audio] Failed to load: " << path << '\n';
			return false;
		}

		SoundPtr ptr(sound.release());
		m_Sounds.emplace(id, std::move(ptr));

		return true;
	}

	void AudioHandler::Unload(uint32_t id)
	{
		m_Sounds.erase(id);
	}

	void AudioHandler::Play(uint32_t id, bool loop, float volume)
	{
		auto it = m_Sounds.find(id);

		if (it == m_Sounds.end())
			return;

		ma_sound* s = it->second.get();

		ma_sound_seek_to_pcm_frame(s, 0);
		ma_sound_set_looping(s, loop ? MA_TRUE : MA_FALSE);
		ma_sound_set_volume(s, volume);
		ma_sound_start(s);
	}

	void AudioHandler::Play(std::string_view path)
	{
		ma_engine_play_sound(&m_SoundEngine, std::string(path).c_str(), nullptr);
	}

	void AudioHandler::Stop(uint32_t id)
	{
		auto it = m_Sounds.find(id);

		if (it == m_Sounds.end())
			return;

		ma_sound* s = it->second.get();
		ma_sound_stop(s);
		ma_sound_seek_to_pcm_frame(s, 0);
	}

	void AudioHandler::Pause(uint32_t id)
	{
		auto it = m_Sounds.find(id);

		if (it == m_Sounds.end())
			return;

		ma_sound_stop(it->second.get());
	}

	void AudioHandler::Resume(uint32_t id)
	{
		auto it = m_Sounds.find(id);

		if (it == m_Sounds.end())
			return;

		ma_sound_start(it->second.get());
	}

	bool AudioHandler::IsPlaying(uint32_t id) const
	{
		auto it = m_Sounds.find(id);

		if (it == m_Sounds.end())
			return false;

		return ma_sound_is_playing(it->second.get()) == MA_TRUE;
	}

	void AudioHandler::SetVolume(uint32_t id, float volume)
	{
		auto it = m_Sounds.find(id);

		if (it == m_Sounds.end())
			return;

		ma_sound_set_volume(it->second.get(), volume);
	}

	float AudioHandler::GetVolume(uint32_t id) const
	{
		auto it = m_Sounds.find(id);

		if (it == m_Sounds.end())
			return 0.0f;

		return ma_sound_get_volume(it->second.get());
	}

	void AudioHandler::SetMasterVolume(float volume)
	{
		m_MasterVolume = volume;
		ma_engine_set_volume(&m_SoundEngine, volume);
	}

	float AudioHandler::GetMasterVolume() const
	{
		return m_MasterVolume;
	}

	void AudioHandler::StopAll()
	{
		for (auto& [_, sound] : m_Sounds)
			ma_sound_stop(sound.get());
	}
}
