
class Game {
    private:
        int _id;
        string _image;
        string _title;
        string _genre;
        string _rating;
        string _author;
        string _exe;
        string _folder;

    public:
        // Default Constructor
        Game(){}

        // Overloaded Constructor
        Game(int id, string image, string title, string genre, string rating, string author, string exe, string folder)
        {
            _id = id;
            _image = image;
            _title = title;
            _genre = genre;
            _rating = rating;
            _author = author;
            _exe = exe;
            _folder = folder;
        }

        //Getters
        auto id() const -> const int& { return _id; }
        auto image() const -> const string& { return _image; }
        auto title() const -> const string& { return _title; }
        auto genre() const -> const string& { return _genre; }
        auto rating() const -> const string& { return _rating; }
        auto author() const -> const string& { return _author; }
        auto exe() const -> const string& { return _exe; }
        auto folder() const -> const string& { return _folder; }
};