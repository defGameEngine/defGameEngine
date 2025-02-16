#pragma once

#ifndef DGE_CONSOLE_HPP
#define DGE_CONSOLE_HPP

#include <vector>

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
            std::string command;
            std::string output;

            Pixel outputColour;
        };

        Console();

        void Clear();
        void Show(bool show);
        void SetBackgroundColour(const Pixel& colour);

        bool IsShown() const;

    protected:
        void HandleCommand(const std::string& command);
        void HandleHistoryBrowsing();
        void Draw();

        void SetLayer(Layer* layer, size_t index);

    private:
        Pixel m_BackgroundColour;

        std::vector<Entry> m_History;
		size_t m_PickedHistoryCommand;

        Layer* m_Layer;
        size_t m_LayerIndex;

    };
}

#endif
