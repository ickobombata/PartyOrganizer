import React from 'react';
import { StyleSheet, Text, View } from 'react-native';

import Login from './app/components/Login';

export default class App extends React.Component {
  state = {
    isLoggedIn: false
  }
  
  render() {
    return (
      <Login onLoginPress={() => this.setState({isLoggedIn: true})} />
    );
  }
}
