/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#pragma once

#ifndef DGE_AUDIO_HPP
#define DGE_AUDIO_HPP

#include "Pch.hpp"

namespace def
{
	// Manages audio playback for sounds and music.
	// Sounds are identified by a user-defined string id.
	// Use Load() to register a file, then Play/Stop/Pause/Resume by id.
	// PlayOneShot() plays a file with no tracking — good for quick SFX.
	class AudioHandler
	{
	public:
		friend class GameEngine;

		// Load a sound file and associate it with id.
		// Supported formats: WAV, MP3, FLAC, OGG.
		// Returns false if the file could not be opened.
		bool Load(std::string_view id, std::string_view path);

		// Unload a previously loaded sound and free its resources.
		void Unload(std::string_view id);

		// Play a loaded sound. loop = true for BGM-style looping.
		// Restarts from the beginning if called while already playing.
		void Play(std::string_view id, bool loop = false, float volume = 1.0f);

		// Play a file as a fire-and-forget one-shot.
		// No id is assigned — the sound plays once and is discarded.
		void PlayOneShot(std::string_view path);

		// Stop playback and rewind to the start.
		void Stop(std::string_view id);

		// Pause without rewinding; Resume() continues from the same position.
		void Pause(std::string_view id);
		void Resume(std::string_view id);

		// Returns true if the sound is currently playing.
		bool IsPlaying(std::string_view id) const;

		// Per-sound volume in [0.0, 1.0].
		void SetVolume(std::string_view id, float volume);
		float GetVolume(std::string_view id) const;

		// Master volume applied to all sounds (default 1.0).
		void SetMasterVolume(float volume);
		float GetMasterVolume() const;

		// Stop all currently playing sounds simultaneously.
		void StopAll();

	protected:
		void Init();
		void Destroy();

	private:
		// Pimpl: keeps miniaudio types out of this header.
		struct Impl;
		std::unique_ptr<Impl> m_Impl;

		float m_MasterVolume = 1.0f;
	};
}

#endif
