/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#include "Pch.hpp"

// miniaudio implementation — compiled exactly once here
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "Audio.hpp"

namespace def
{
	struct AudioHandler::Impl
	{
		ma_engine engine;

		// Each loaded sound lives on the heap so its address never changes
		// after ma_sound_init (miniaudio stores internal self-pointers).
		struct MaSoundDeleter
		{
			void operator()(ma_sound* s) const
			{
				ma_sound_uninit(s);
				delete s;
			}
		};

		using SoundPtr = std::unique_ptr<ma_sound, MaSoundDeleter>;
		std::unordered_map<std::string, SoundPtr> sounds;
	};

	void AudioHandler::Init()
	{
		m_Impl = std::make_unique<Impl>();

		if (ma_engine_init(nullptr, &m_Impl->engine) != MA_SUCCESS)
			std::cerr << "[Audio] Failed to initialise audio engine\n";
	}

	void AudioHandler::Destroy()
	{
		if (!m_Impl)
			return;

		m_Impl->sounds.clear();
		ma_engine_uninit(&m_Impl->engine);
		m_Impl.reset();
	}

	bool AudioHandler::Load(std::string_view id, std::string_view path)
	{
		auto key = std::string(id);
		auto it = m_Impl->sounds.find(key);

		if (it != m_Impl->sounds.end())
		{
			ma_sound_uninit(it->second.get());
			m_Impl->sounds.erase(it);
		}

		auto sound = std::make_unique<ma_sound>();

		ma_result result = ma_sound_init_from_file(
			&m_Impl->engine,
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

		// Transfer ownership with the custom deleter
		Impl::SoundPtr ptr(sound.release());
		m_Impl->sounds.emplace(key, std::move(ptr));
		return true;
	}

	void AudioHandler::Unload(std::string_view id)
	{
		m_Impl->sounds.erase(std::string(id));
	}

	void AudioHandler::Play(std::string_view id, bool loop, float volume)
	{
		auto it = m_Impl->sounds.find(std::string(id));
		if (it == m_Impl->sounds.end())
			return;

		ma_sound* s = it->second.get();
		ma_sound_seek_to_pcm_frame(s, 0);
		ma_sound_set_looping(s, loop ? MA_TRUE : MA_FALSE);
		ma_sound_set_volume(s, volume);
		ma_sound_start(s);
	}

	void AudioHandler::PlayOneShot(std::string_view path)
	{
		ma_engine_play_sound(&m_Impl->engine, std::string(path).c_str(), nullptr);
	}

	void AudioHandler::Stop(std::string_view id)
	{
		auto it = m_Impl->sounds.find(std::string(id));
		if (it == m_Impl->sounds.end())
			return;

		ma_sound* s = it->second.get();
		ma_sound_stop(s);
		ma_sound_seek_to_pcm_frame(s, 0);
	}

	void AudioHandler::Pause(std::string_view id)
	{
		auto it = m_Impl->sounds.find(std::string(id));
		if (it == m_Impl->sounds.end())
			return;

		ma_sound_stop(it->second.get());
	}

	void AudioHandler::Resume(std::string_view id)
	{
		auto it = m_Impl->sounds.find(std::string(id));
		if (it == m_Impl->sounds.end())
			return;

		ma_sound_start(it->second.get());
	}

	bool AudioHandler::IsPlaying(std::string_view id) const
	{
		auto it = m_Impl->sounds.find(std::string(id));
		if (it == m_Impl->sounds.end())
			return false;

		return ma_sound_is_playing(it->second.get()) == MA_TRUE;
	}

	void AudioHandler::SetVolume(std::string_view id, float volume)
	{
		auto it = m_Impl->sounds.find(std::string(id));
		if (it == m_Impl->sounds.end())
			return;

		ma_sound_set_volume(it->second.get(), volume);
	}

	float AudioHandler::GetVolume(std::string_view id) const
	{
		auto it = m_Impl->sounds.find(std::string(id));
		if (it == m_Impl->sounds.end())
			return 0.0f;

		return ma_sound_get_volume(it->second.get());
	}

	void AudioHandler::SetMasterVolume(float volume)
	{
		m_MasterVolume = volume;
		ma_engine_set_volume(&m_Impl->engine, volume);
	}

	float AudioHandler::GetMasterVolume() const
	{
		return m_MasterVolume;
	}

	void AudioHandler::StopAll()
	{
		for (auto& [id, sound] : m_Impl->sounds)
			ma_sound_stop(sound.get());
	}
}
