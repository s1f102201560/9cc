#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc ./tmp.s -o tmp
  ./tmp

  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $atctual"
    exit 1
  fi
}

assert 42 42
assert 0 0
assert 21 "5+20-4"

echo OK