#ifndef ARCADE_MACHINE_BUTTON_LIST_H
#define ARCADE_MACHINE_BUTTON_LIST_H

#include "ConfigData.h"
#include "GameData.h"

class ButtonNode
{
private:
    ButtonNode *next;
    ButtonNode *prev;

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
        this->next = this;
        this->prev = this;
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
        this->next = next;
        this->prev = prev;
    }

    /**
     * @brief add a node after the current node
     * 
     * @param button the button to be linked to the node
     */
    void addAfter(ButtonNode *node)
    {
        node->next = this->next;
        node->prev = this;
        this->next->prev = node;
        this->next = node;
    }

    /**
     * @brief add a node before the current node
     * 
     * @param button the button to be linked to the node
     */
    void addBefore(ButtonNode *node)
    {
        node->next = this;
        node->prev = this->prev;
        this->prev->next = node;
        this->prev = node;
    }

    /**
     * @brief remove this node from the list
     * 
     */
    void remove()
    {
        this->prev->next = this->next;
        this->next->prev = this->prev;
    }

    /**
     * @brief get the next node in the list
     * 
     * @return ButtonNode* the next node in the list
     */
    ButtonNode *getNext()
    {
        return this->next;
    }
    
    /**
     * @brief get the previous node in the list
     * 
     * @return ButtonNode* the previous node in the list
     */
    ButtonNode *getPrev()
    {
        return this->prev;
    }
};

#endif