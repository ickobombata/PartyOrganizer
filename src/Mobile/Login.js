import React, { Component } from 'react';
import {
    ScrollView,
    Text,
    TextInput,
    View,
    Button,
    ActivityIndicator
} from 'react-native';

export default class Login extends Component {
    state = {
        username: '',
        password: '',
        isLoggingIn: false,
        message: ''
    }

    UserLogin = () => {
        this.setState({isLoggingIn: true, message:''});

        var params = {
            username: this.state.username,
            password: this.state.password
        };
        
        console.log(JSON.stringify(params))
        fetch('http://192.168.0.104:12345/token/generate', {
            method: 'POST',
            headers: {
                Accept: 'application/json',
                'Content-Type': 'application/json'
            },
            body: '{"username": "angie","password": "1234"}'
        })
        .then((response) => response.json())
        .then((response) => {
            console.log(response)
          if (response.error) this.setState({message: response.message});
          else proceed = true;

        })
        .then(() => {
          this.setState({isLoggingIn: false})
          if (proceed) this.props.onLoginPress();
        })
        .done();
    }

    render() {
        return (
            <ScrollView style={{padding: 20}}>
                <Text 
                    style={{fontSize: 27}}>
                    Login
                </Text>
                <TextInput placeholder='Username' onChangeText={(username) => this.setState({username})} />
                <TextInput placeholder='Password' onChangeText={(password) => this.setState({password})} />
                <View style={{margin:7}} />

                {!!this.state.message && (
                    <Text style={{fontSize: 14, color: 'red', padding: 5}}>
                        {this.state.message}
                    </Text>
                )}
                {this.state.isLoggingIn && <ActivityIndicator />}
                <Button 
                        onPress={this.UserLogin}
                        title="Submit"
                    />
            </ScrollView>
        )
    }
}
