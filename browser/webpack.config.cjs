const path = require('path');

module.exports = {
  entry: './src/index.ts',
  module: {
    rules: [
      {test: /\.ts/, use: 'ts-loader', exclude: /node_modules/}
    ]
  },
  resolve: {
    extensions: [
      '.ts',
      '.js'
    ]
  },
  output: {
    path: path.resolve(__dirname, 'bundle'),
    filename: 'browser.js'
  }
};