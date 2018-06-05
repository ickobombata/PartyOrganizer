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
        
        fetch('http://192.168.0.104:12345/token/generate', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(params)
        })
        .then((response) => response.json() )
        .then((response) => {
            console.log(response)
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
