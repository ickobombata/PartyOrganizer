import React, { Component } from 'react';
import {
    ScrollView,
    Text,
    TextInput,
    View,
    Button,
    ActivityIndicator,
    StyleSheet,
    ImageBackground
} from 'react-native';

import * as settings from './Settings';
console.log(settings.apiUrl);

export default class Login extends Component {
    state = {
        username: '',
        password: '',
        isLoggingIn: false,
        message: ''
    }

    UserLogin = () => {
        console.log('Logging in...');
        this.setState({ isLoggingIn: true, message: '' });

        var params = {
            username: this.state.username,
            password: this.state.password
        };
    
        fetch(`${settings.apiUrl}/token/generate`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(params)
        })
            .then((response) => response.json())
            .then((response) => {
                console.log(response)
                this.setState({ isLoggingIn: false })
                alert(`Received token ${response.token}`)
            })
            .then((error) => console.log(error))
            .done();
    }

    render() {
        return (
            <ImageBackground style={styles.background} source={require('./content/picnic.jpg')}>
                <View style={styles.overlay} />
                <Text style={styles.title}>
                    Party Organizer
                </Text>
                <ScrollView style={styles.container} contentContainerStyle={styles.contentContainer}>
                    <Text
                        style={styles.login}>
                        Login
                </Text>
                    <TextInput style={styles.text} placeholder='Username'
                        onChangeText={(username) => this.setState({ username })}
                        underlineColorAndroid='white'/>
                    <TextInput style={styles.text} placeholder='Password'
                        onChangeText={(password) => this.setState({ password })}
                        underlineColorAndroid='lightgray'/>
                    <View style={{ margin: 7 }} />

                    {!!this.state.message && (
                        <Text style={{ fontSize: 14, color: 'red', padding: 5 }}>
                            {this.state.message}
                        </Text>
                    )}
                    {this.state.isLoggingIn && <ActivityIndicator size='large'/>}
                    <Button
                        onPress={this.UserLogin}
                        title="Submit"
                    />
                </ScrollView>
            </ImageBackground>
        )
    }
}

const styles = StyleSheet.create({
    container: {
        padding: 20
    },
    contentContainer: {
        flexGrow: 10,
        justifyContent: 'center'
    },
    text: {
        height: 50,
        fontSize: 20,
        color: 'white'
    },
    overlay: {
        position: 'absolute',
        top: 0,
        right: 0, 
        bottom: 0,
        left: 0,
        backgroundColor: 'black',
        opacity: 0.3
    },
    title: { 
        fontSize: 40,
        color: 'white',
        margin: 30,
        textAlign: 'center'
    },
    background: {
        flex: 1,
        flexDirection: 'column'
    },
    login: {
        fontSize: 27,
        color: 'white'
    }
})
