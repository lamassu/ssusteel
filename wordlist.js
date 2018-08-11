const fs = require('fs')
const _ = require('lodash/fp')
const util = require('util')

const wordList = fs.readFileSync('./wordlist.txt', 'utf8')

const words = _.map(_.truncate({length: 4, omission: ''}), wordList.split('\n').slice(0, -1))

function occuranceCount (letter, str) {
  return str.split(letter).length - 1
}

const occurances = _.curry(occuranceCount)

const letterArr = []

for (let i = 0; i < 26; i++) {
  const ascii = 97 + i
  const letter = String.fromCharCode(ascii)
  const sortedWords = _.sortBy(occurances(letter), words)
  const top12 = _.takeRight(12, sortedWords)
  const total = _.sumBy(occurances(letter), top12)

  letterArr[i] = {letter, total, top12}
}

const getMax = _.flow(_.map('total'), _.max)

const chunks = _.chunk(6, _.reverse(_.sortBy('total', letterArr)))
const totals = _.map(getMax, chunks)
const sum = _.sum(totals)

function pp (s) {
  console.log(util.inspect(s, {colors: true, depth: null}))
}

pp(chunks)
console.log(totals)
console.log(sum)
