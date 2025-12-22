#include "core/Lexer.h"
#include "core/Parser.h"

// TIP 要<b>Run</b>代码，请按 <shortcut actionId="Run"/> 或点击装订区域中的 <icon src="AllIcons.Actions.Execute"/> 图标。
int main() {
    std::string source, line;
    while (std::getline(std::cin, line)) {
        source += line + '\n';
    }
    Lexer lexer(source, true);
    auto tokens = lexer.tokenize();
    Parser parser(tokens, true);
    auto expression = parser.parse_expression();
    auto statements = parser.parse();
    for (auto statement : statements) {
        statement->print();
    }

    system("pause");

    return 0;
    // TIP 请访问 <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a> 查看 CLion 帮助。此外，您还可以从主菜单中选择“帮助 | 学习 IDE 功能”，尝试 CLion 的交互式课次。
}