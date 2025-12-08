#include "ExamApplication.h"

int main(int argc, char* argv[])
{

    // Create the Assignment application
    ExamApplication application("Exam", "1.0");

    // Initialize
    if (application.Init() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    // Run the application
    return application.Run();
}