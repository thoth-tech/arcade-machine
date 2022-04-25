class ButtonNode
{
private:
    ButtonNode *next;
    ButtonNode *prev;

public:
    Button *button;
    //Create a new node with the given data
    ButtonNode(Button *button)
    {
        this->button = button;
        this->next = this;
        this->prev = this;
    }
    //Create a new node with the given data and link it
    ButtonNode(Button *button, ButtonNode *next, ButtonNode *prev)
    {
        this->button = button;
        this->next = next;
        this->prev = prev;
    }

    //Add a new node after this node
    void addAfter(ButtonNode *node)
    {
        node->next = this->next;
        node->prev = this;
        this->next->prev = node;
        this->next = node;
    }

    //Add a new node before this node
    void addBefore(ButtonNode *node)
    {
        node->next = this;
        node->prev = this->prev;
        this->prev->next = node;
        this->prev = node;
    }

    //Remove this node from the list
    void remove()
    {
        this->prev->next = this->next;
        this->next->prev = this->prev;
    }

    //Get the next node in the list
    ButtonNode *getNext()
    {
        return this->next;
    }

    //Get the previous node in the list
    ButtonNode *getPrev()
    {
        return this->prev;
    }
};
