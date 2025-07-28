#pragma once

#ifndef DGE_CONSOLE_HPP
#define DGE_CONSOLE_HPP

#include "Pch.hpp"
#include "Pixel.hpp"
#include "Layer.hpp"
#include "defGameEngine.hpp"

namespace def
{
    class Console
    {
    public:
        friend class InputHandler;
        friend class GameEngine;

    public:
        struct Entry
        {
            std::string command; // not used if isCommand = false
            std::string output;

            Pixel outputColour;

            bool isCommand = false;
        };

        Console();

        void Clear();
        void Print(const std::string& text, const Pixel& colour = def::WHITE);
        void Show(bool show);
        void SetBackgroundColour(const Pixel& colour);

        bool IsShown() const;

    protected:
        void HandleCommand(const std::string& command);
        void HandleHistoryBrowsing();
        void Draw();

    private:
        Pixel m_BackgroundColour;

        std::vector<Entry> m_History;
		size_t m_PickedHistoryCommand;

    };
}

#endif
