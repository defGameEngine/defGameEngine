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
            // Stores user's prompt and is not used if isCommand = false.
            std::string command;
            
            // Output message.
            std::string output;

            // Colour of the output message.
            Pixel outputColour;

            // True if the entry is a command and
            // false if the entry is a regular text
            // that was printed to the console.
            bool isCommand = false;
        };

        Console();

        // Clears the console and its history.
        void Clear();
        
        // Prints a text to the console with the specified colour.
        void Print(const std::string& text, const Pixel& colour = def::WHITE);

        // Shows or hides the console.
        void Show(bool show);

        // Sets the background colour of the console.
        void SetBackgroundColour(const Pixel& colour);

        // Returns true if the console is visible.
        bool IsShown() const;

    protected:
        void HandleCommand(const std::string& command);
        void HandleHistoryBrowsing();
        void Draw();

    private:
        Pixel m_BackgroundColour;

        std::vector<Entry> m_History;

        // Index of the current command from the history (m_History),
        // it's set to m_History.size() if the command hasn't been
        // selected yet.
		size_t m_PickedHistoryCommand;

    };
}

#endif
