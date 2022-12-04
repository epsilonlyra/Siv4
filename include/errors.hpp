#ifndef H_ERROR
#define H_ERROR
#include<string>


 struct base_exception {
        std :: string virtual what() = 0;
};

struct out_of_boundary_change final : base_exception {
        std::string what() override;
};

struct font_not_loaded final  : base_exception {
        std::string what() override;
};


#endif
