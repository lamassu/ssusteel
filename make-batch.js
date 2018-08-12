const num = process.argv[2]

for (let i = 0; i < num; i++) {
  console.log(`./a.out ${i} &`)
}
