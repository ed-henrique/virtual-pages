FROM alpine AS builder

WORKDIR /app

COPY . .

RUN apk update && apk add --no-cache gcc libc-dev
RUN gcc -O3 -Wall -Wextra -o main main.c

FROM alpine AS exec

WORKDIR /app

COPY --from=builder /app/main . 
COPY --from=builder /app/big-file.txt . 

ENTRYPOINT ["/app/main"]

