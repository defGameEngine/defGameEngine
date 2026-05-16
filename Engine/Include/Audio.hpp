/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#pragma once

#ifndef DGE_AUDIO_HPP
#define DGE_AUDIO_HPP

#include "Pch.hpp"

#include "miniaudio.h"

namespace def
{
	// Manages audio playback for sounds and music,
	// sounds are identified by a user-defined numeric ID
	class AudioHandler
	{
	public:
		friend class GameEngine;

		AudioHandler();
		~AudioHandler();

		// Load a sound file and associate it with id,
		// supported formats: WAV, MP3, FLAC, OGG,
		// returns false if the file could not be opened
		bool Load(uint32_t id, std::string_view path);

		// Unload a previously loaded sound and free its resources
		void Unload(uint32_t id);

		// Play a loaded sound, loop = true for BGM-style looping,
		// restarts from the beginning if called while already playing
		void Play(uint32_t id, bool loop = false, float volume = 1.0f);

		// Play a file as a fire-and-forget one-shot,
		// no ID is assigned — the sound plays once and is discarded
		void Play(std::string_view path);

		// Stop playback and rewind to the start
		void Stop(uint32_t id);

		// Pause without rewinding
		void Pause(uint32_t id);

		// Continues from the position where the sound was stopped by Pause
		void Resume(uint32_t id);

		// Returns true if the sound is currently playing
		bool IsPlaying(uint32_t id) const;

		// Per-sound volume in [0.0, 1.0]
		void SetVolume(uint32_t id, float volume);
		float GetVolume(uint32_t id) const;

		// Master volume applied to all sounds (default 1.0)
		void SetMasterVolume(float volume);
		float GetMasterVolume() const;

		// Stop all currently playing sounds simultaneously
		void StopAll();

	private:
		ma_engine m_SoundEngine;

		struct MaSoundDeleter
		{
			void operator()(ma_sound* s) const
			{
				ma_sound_uninit(s);
				delete s;
			}
		};

		using SoundPtr = std::unique_ptr<ma_sound, MaSoundDeleter>;
		std::unordered_map<uint32_t, SoundPtr> m_Sounds;

		float m_MasterVolume = 1.0f;
	};
}

#endif
