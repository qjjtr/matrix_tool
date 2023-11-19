#include "args_parser.h"
#include "matrix.h"

#include <iostream>

enum class Action {
    INVERT,
    DETERMINANT,
    ADD,
    SUB,
    MULTIPLY
};

Matrix ReadMatrix() {
    std::cout << "Enter height and width:" << std::endl;
    size_t n, m;
    std::cin >> n >> m;
    std::cout << "Enter elements:" << std::endl;
    std::vector<std::vector<Poly>> matrix(n, std::vector<Poly>(m));
    for (auto& line : matrix) {
        for (auto& elem : line) {
            std::cin >> elem;
        }
    }
    return Matrix(matrix);
}

void PrintMatrix(const Matrix& matrix, bool latex) {
    if (latex) {
        std::cout << "\\begin{pmatrix}" << std::endl;
    }

    for (const auto& line : matrix.GetData()) {
        bool isFirst = true;
        for (const auto& element : line) {
            if (!isFirst) {
                std::cout << (latex ? " & " : " ");
            }
            isFirst = false;
            auto str = element.AsString();
            if (latex) {
                if (str.find('x') != std::string::npos) {
                    throw "can't format poly as latex";
                }
                auto slash = str.find('/');
                if (slash != std::string::npos) {
                    str = "\\frac{" + str.substr(0, slash) + "}{" + str.substr(slash + 1) + "}";
                }
            }
            std::cout << str;
        }
        if (latex) {
            std::cout << " \\\\";
        }
        std::cout << std::endl;
    }

    if (latex) {
        std::cout << "\\end{pmatrix}" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    Action action;
    bool latex;
    ArgsParser{}
        .AddLongOption<Action>('a', "action", &action, true,
            "One of: INVERT, DETERMINANT, ADD, SUB, MULTIPLY",
            [] (const std::string& str) {
                switch (str[0]) {
                    case 'I': return Action::INVERT;
                    case 'D': return Action::DETERMINANT;
                    case 'A': return Action::ADD;
                    case 'S': return Action::SUB;
                    case 'M': return Action::MULTIPLY;
                    default:
                        throw "Unknown option";
                }
            })
        .AddLongOption('l', "latex", &latex, false, "print result matrix in latex format")
        .SetHelpMessage("Some actions with matrices. Matrix element is poly with fractions. Write poly without spaces, fractions with /.")
        .Parse(argc, argv);

    try {
        switch (action) {
            case Action::INVERT: {
                PrintMatrix(ReadMatrix().Inverted(), latex);
                break;
            }
            case Action::DETERMINANT: {
                std::cout << ReadMatrix().Determinant() << std::endl;
                break;
            }
            case Action::ADD: {
                auto A = ReadMatrix();
                auto B = ReadMatrix();
                PrintMatrix(A + B, latex);
                break;
            }
            case Action::SUB: {
                auto A = ReadMatrix();
                auto B = ReadMatrix();
                PrintMatrix(A - B, latex);
                break;
            }
            case Action::MULTIPLY: {
                auto A = ReadMatrix();
                auto B = ReadMatrix();
                PrintMatrix(A * B, latex);
                break;
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Exception occurred: " << e.what() << std::endl;
    } catch (const char* str) {
        std::cout << "Exception occurred: " << str << std::endl;
    } catch (...) {
        std::cout << "Some exception occurred" << std::endl;
    }
}
