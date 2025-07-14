# CS_Assignment

이 저장소는 컴퓨터 구조 과제 관련 파일을 올려놓은 저장소입니다.



# MIPS 어셈블리 코드 변환기

## 개요

이 프로그램은 MIPS 어셈블리 코드를 이진(Binary) 코드로 변환하는 기능을 수행합니다. MIPS 명령어 중 I-format과 R-format 명령어를 처리하며, 변환된 결과를 `output.bin` 파일에 저장합니다.

## 주요 기능

1. **I-format 명령어 변환**
   - `addi`, `andi`, `ori`, `xori`, `lw`, `sw`, `lui`, `slti`, `beq`, `bne` 등의 I-format 명령어를 변환합니다.
2. **R-format 명령어 변환**
   - `add`, `sub`, `and`, `or`, `xor`, `nor`, `slt`, `sll`, `srl`, `jr` 등의 R-format 명령어를 변환합니다.
3. **레지스터 변환**
   - MIPS 레지스터(`$zero`, `$v0`, `$a0` 등)를 5비트 이진 코드로 변환합니다.
4. **어셈블리 코드 읽기 및 변환**
   - 주어진 어셈블리 코드 파일(`input.asm`)을 읽고 변환한 후, `output.bin` 파일에 저장합니다.

## 코드 설명

### 1. 명령어 및 레지스터 매핑

- `iformat_opcode` : I-format 명령어를 6비트 opcode로 매핑합니다.
- `rformat_function` : R-format 명령어를 6비트 function code로 매핑합니다.
- `registers` : MIPS 레지스터를 5비트 이진 값으로 변환합니다.

### 2. I-format 변환 (`iformat` 구조체)

- opcode, rs, rt, immediate 값을 변환하여 32비트 명령어를 생성합니다.
- `immediate_to_binary()` 함수는 16비트 immediate 값을 이진 문자열로 변환합니다.

### 3. R-format 변환 (`rformat` 구조체)

- opcode(고정값 `000000`), rs, rt, rd, shamt(고정값 `00000`), funct 값을 변환하여 32비트 명령어를 생성합니다.

### 4. 어셈블리 코드 파싱 및 변환 (`parse_line` 함수)

- 한 줄씩 읽어 `tokenize()` 함수로 토큰화합니다.
- 변환할 명령어 유형을 판별한 후 적절한 구조체를 사용하여 변환합니다.
- 변환된 이진 명령어를 `output.bin` 파일에 저장합니다.

### 5. 실행 (`main` 함수)

- `input.asm` 파일을 열어 어셈블리 코드를 읽습니다.
- 변환된 코드를 `output.bin` 파일에 저장합니다.
- 실행 중 오류가 발생하면 적절한 에러 메시지를 출력합니다.

## 실행 방법

1. `input.asm` 파일을 준비합니다.
2. 프로그램을 실행하면 `output.bin` 파일이 생성됩니다.

## 주의사항

- 현재 프로그램은 기본적인 명령어만 지원합니다.
- immediate 값은 16비트 정수 범위 내에서 처리됩니다.
- 어셈블리 코드 형식이 올바르지 않으면 변환이 실패할 수 있습니다.

