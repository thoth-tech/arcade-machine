#ifndef ARCADE_MACHINE_BUTTON_NODE_H
#define ARCADE_MACHINE_BUTTON_NODE_H

#include "ConfigData.h"
#include "Button.h"
#include "GameData.h"

class ButtonNode
{
private:
    ButtonNode *m_next;
    ButtonNode *m_prev;

public:
    Button *button;
    ConfigData config;
    GameData stats;

    /**
     * @brief construct a new Button Node object
     * 
     * @param button the button to be stored in the node
     */
    ButtonNode(Button *button)
    {
        this->button = button;
        this->m_next = this;
        this->m_prev = this;
    }

    /**
     * @brief construct a new Button Node object
     * 
     * @param button the button to be stored in the node
     * @param next the next node
     * @param prev the previous node
     */
    ButtonNode(Button *button, ButtonNode *next, ButtonNode *prev)
    {
        this->button = button;
        this->m_next = next;
        this->m_prev = prev;
    }

    /**
     * @brief add a node after the current node
     * 
     * @param button the button to be linked to the node
     */
    void addAfter(ButtonNode *node)
    {
        node->m_next = this->m_next;
        node->m_prev = this;
        this->m_next->m_prev = node;
        this->m_next = node;
    }

    /**
     * @brief add a node before the current node
     * 
     * @param button the button to be linked to the node
     */
    void addBefore(ButtonNode *node)
    {
        node->m_next = this;
        node->m_prev = this->m_prev;
        this->m_prev->m_next = node;
        this->m_prev = node;
    }

    /**
     * @brief remove this node from the list
     * 
     */
    void remove()
    {
        this->m_prev->m_next = this->m_next;
        this->m_next->m_prev = this->m_prev;
    }

    /**
     * @brief get the next node in the list
     * 
     * @return ButtonNode* the next node in the list
     */
    ButtonNode *getNext()
    {
        return this->m_next;
    }
    
    /**
     * @brief get the previous node in the list
     * 
     * @return ButtonNode* the previous node in the list
     */
    ButtonNode *getPrev()
    {
        return this->m_prev;
    }
};

#endif