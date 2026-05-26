#!/bin/bash

# Cores para o output
GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[0;33m"
RESET="\033[0m"

MINISHELL="./minishell"
TMP_MINI="mini.out"
TMP_BASH="bash.out"

# Verificar se o executável existe
if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Erro: Executável '$MINISHELL' não encontrado. Compila primeiro com 'make'.${RESET}"
    exit 1
fi

total_tests=0
passed_tests=0

run_test() {
    local category=$1
    local test_cmd=$2
    ((total_tests++))

    # Executa no Minishell (enviando o comando via stdin e simulando saída)
    echo "$test_cmd" | $MINISHELL > $TMP_MINI 2>&1
    local mini_status=$?

    # Executa na Bash
    echo "$test_cmd" | bash > $TMP_BASH 2>&1
    local bash_status=$?

    # Compara o output e o status de saída
    diff $TMP_MINI $TMP_BASH > /dev/null
    local diff_out=$?

    if [ $diff_out -eq 0 ] && [ $mini_status -eq $bash_status ]; then
        echo -e "[${category}] '${test_cmd}' -> ${GREEN}OK${RESET}"
        ((passed_tests++))
    else
        echo -e "[${category}] '${test_cmd}' -> ${RED}KO${RESET}"
        if [ $diff_out -ne 0 ]; then
            echo -e "  ${YELLOW}Diferença no Output:${RESET}"
            diff -u $TMP_MINI $TMP_BASH | sed 's/^/  /'
        fi
        if [ $mini_status -ne $bash_status ]; then
            echo -e "  ${YELLOW}Diferença no Exit Status: Minishell ($mini_status) vs Bash ($bash_status)${RESET}"
        fi
    fi
}

# Limpeza inicial de ficheiros temporários que possam existir
rm -f $TMP_MINI $TMP_BASH test_file*.txt

echo -e "${YELLOW}=== INICIANDO TESTES MINISHELL (SEM BÓNUS) ===${RESET}\n"

# ---------------------------------------------------------------------------
# 1. COMANDOS SIMPLES E PATH
# ---------------------------------------------------------------------------
run_test "PATH" "ls"
run_test "PATH" "uname -a"
run_test "PATH" "/bin/ls -la"
run_test "PATH" "absolute_path_fail"
run_test "PATH" ""

# ---------------------------------------------------------------------------
# 2. ASPAS (Quotes)
# ---------------------------------------------------------------------------
run_test "QUOTES" "echo 'hello world'"
run_test "QUOTES" "echo \"hello world\""
run_test "QUOTES" "echo '\"hello\"'"
run_test "QUOTES" "echo \"'hello'\""

# ---------------------------------------------------------------------------
# 3. REDIRECIONAMENTOS (<, >, >>, <<)
# ---------------------------------------------------------------------------
run_test "REDIR" "echo texto_teste > test_file.txt"
run_test "REDIR" "cat test_file.txt"
run_test "REDIR" "echo mais_texto >> test_file.txt"
run_test "REDIR" "cat < test_file.txt"
run_test "REDIR" "cat < test_file.txt > test_file_copy.txt"
run_test "REDIR" "cat test_file_copy.txt"
# Here-doc simples
run_test "HERE-DOC" "cat << EOF
linha1
linha2
EOF"

# ---------------------------------------------------------------------------
# 4. PIPES (|)
# ---------------------------------------------------------------------------
run_test "PIPES" "ls -l | grep test"
run_test "PIPES" "cat test_file.txt | grep texto | wc -l"
run_test "PIPES" "echo ola | cat | cat | cat"

# ---------------------------------------------------------------------------
# 5. EXPANSÃO DE VARIÁVEIS E $?
# ---------------------------------------------------------------------------
run_test "ENV_VAR" "echo \$USER"
run_test "ENV_VAR" "echo \$HOME"
run_test "ENV_VAR" "echo \"\$USER\""
run_test "ENV_VAR" "echo '\$USER'" # Não deve expandir dentro de aspas simples
run_test "STATUS" "ls /ficheiro_nao_existe"
run_test "STATUS" "echo \$?"
run_test "STATUS" "expr 1 + 1"
run_test "STATUS" "echo \$?"

# ---------------------------------------------------------------------------
# 6. BUILTINS
# ---------------------------------------------------------------------------

# ECHO
run_test "ECHO" "echo"
run_test "ECHO" "echo -n sem nova linha"
run_test "ECHO" "echo -n -n -n multiplos n"
run_test "ECHO" "echo-n colado"

# PWD e CD
run_test "PWD" "pwd"
run_test "CD" "cd .."
run_test "CD" "pwd"
run_test "CD" "cd -" # Depende se implementaste o histórico de cd, a bash suporta
run_test "CD" "cd /"
run_test "CD" "pwd"

# ENV, EXPORT e UNSET
run_test "EXPORT" "export TEST_VAR=42"
run_test "EXPORT" "echo \$TEST_VAR"
run_test "UNSET" "unset TEST_VAR"
run_test "UNSET" "echo \$TEST_VAR"
run_test "EXPORT" "export INVALID-VAR=123" # Deve dar erro de identificador inválido
run_test "ENV" "env | grep USER" # Apenas para ver se o env básico funciona

# EXIT
run_test "EXIT" "exit"
run_test "EXIT" "exit 42"
run_test "EXIT" "exit 2 3" # Demasiados argumentos
run_test "EXIT" "exit abc" # Argumento numérico necessário

# ---------------------------------------------------------------------------
# LIMPEZA E RESULTADOS
# ---------------------------------------------------------------------------
rm -f $TMP_MINI $TMP_BASH test_file*.txt

echo -e "\n${YELLOW}=== RESUMO DOS TESTES ===${RESET}"
if [ $passed_tests -eq $total_tests ]; then
    echo -e "${GREEN}Sucesso Total! Passaste em $passed_tests/$total_tests testes.${RESET}"
else
    echo -e "${RED}Falhas detetadas. Passaste em $passed_tests de $total_tests testes.${RESET}"
fi