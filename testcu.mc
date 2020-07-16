function main() {
  var i, sum;
  i = 1;
  sum = 0;
  while (i < 10) {
    var sum;
    sum = add(sum, i);
    i = add(i, 1);
  }
  return sum;
}

function add(a1, a2) {
  var a;
  a = a1 + a2;
  return a;
}

