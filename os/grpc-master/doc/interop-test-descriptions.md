Interoperability Test Case Descriptions
=======================================

Client and server use
[test.proto](https://github.com/grpc/grpc/blob/master/test/proto/test.proto)
and the [gRPC over HTTP/2 v2
protocol](doc/PROTOCOL-HTTP2.md).

Client
------

Clients implement test cases that test certain functionally. Each client is
provided the test case it is expected to run as a command-line parameter. Names
should be lowercase and without spaces.

Clients should accept these arguments:
* --server_host=HOSTNAME
    * The server host to connect to. For example, "localhost" or "127.0.0.1"
* --server_host_override=HOSTNAME
    * The server host to claim to be connecting to, for use in TLS and HTTP/2
      :authority header. If unspecified, the value of --server_host will be
      used
* --server_port=PORT
    * The server port to connect to. For example, "8080"
* --test_case=TESTCASE
    * The name of the test case to execute. For example, "empty_unary"
* --use_tls=BOOLEAN
    * Whether to use a plaintext or encrypted connection
* --use_test_ca=BOOLEAN
    * Whether to replace platform root CAs with
      [ca.pem](https://github.com/grpc/grpc/blob/master/src/core/tsi/test_creds/ca.pem)
      as the CA root
* --default_service_account=ACCOUNT_EMAIL
    * Email of the GCE default service account. Only applicable
      for compute_engine_creds test.
* --oauth_scope=SCOPE
    * OAuth scope. For example, "https://www.googleapis.com/auth/xapi.zoo"
* --service_account_key_file=PATH
    * The path to the service account JSON key file generated from GCE developer
    console.

Clients must support TLS with ALPN. Clients must not disable certificate
checking.

### empty_unary

This test verifies that implementations support zero-size messages. Ideally,
client implementations would verify that the request and response were zero
bytes serialized, but this is generally prohibitive to perform, so is not
required.

Server features:
* [EmptyCall][]

Procedure:
 1. Client calls EmptyCall with the default Empty message

Client asserts:
* call was successful
* response is non-null

*It may be possible to use UnaryCall instead of EmptyCall, but it is harder to
ensure that the proto serialized to zero bytes.*

### large_unary

This test verifies unary calls succeed in sending messages, and touches on flow
control (even if compression is enabled on the channel).

Server features:
* [UnaryCall][]
* [Compressable Payload][]

Procedure:
 1. Client calls UnaryCall with:

    ```
    {
      response_type: COMPRESSABLE
      response_size: 314159
      payload:{
        body: 271828 bytes of zeros
      }
    }
    ```

Client asserts:
* call was successful
* response payload type is COMPRESSABLE
* response payload body is 314159 bytes in size
* clients are free to assert that the response payload body contents are zero
  and comparing the entire response message against a golden response

### large_compressed_unary

This test verifies compressed unary calls succeed in sending messages. It
sends one unary request for every combination of compression algorithm and
payload type.

In all scenarios, whether compression was actually performed is determined by
the compression bit in the response's message flags. The response's compression
value indicates which algorithm was used if said compression bit is set.


Server features:
* [UnaryCall][]
* [Compressable Payload][]
* [Uncompressable Payload][]
* [Random Payload][]

Procedure:
 1. Client calls UnaryCall with:

    ```
    {
      response_compression: <one of {NONE, GZIP, DEFLATE}>
      response_type: COMPRESSABLE
      response_size: 314159
      payload:{
        body: 271828 bytes of zeros
      }
    }
    ```
    Client asserts:
    * call was successful
    * response payload type is COMPRESSABLE
    * response compression is consistent with the requested one.
    * if `response_compression == NONE`, the response MUST NOT have the
      compressed message flag set.
    * if `response_compression != NONE`, the response MUST have the compressed
      message flag set.
    * response payload body is 314159 bytes in size
    * clients are free to assert that the response payload body contents are
      zero and comparing the entire response message against a golden response


 2. Client calls UnaryCall with:
    ```
    {
      response_compression: <one of {NONE, GZIP, DEFLATE}>
      response_type: UNCOMPRESSABLE
      response_size: 314159
      payload:{
        body: 271828 bytes of zeros
      }
    }
    ```
    Client asserts:
    * call was successful
    * response payload type is UNCOMPRESSABLE
    * response compression is consistent with the requested one.
    * the response MUST NOT have the compressed message flag set.
    * response payload body is 314159 bytes in size
    * clients are free to assert that the response payload body contents are
      identical to the golden uncompressable data at `test/cpp/interop/rnd.dat`.


 3. Client calls UnaryCall with:
    ```
    {
      response_compression: <one of {NONE, GZIP, DEFLATE}>
      response_type: RANDOM
      response_size: 314159
      payload:{
        body: 271828 bytes of zeros
      }
    }
    ```
    Client asserts:
    * call was successful
    * response payload type is either COMPRESSABLE or UNCOMPRESSABLE
    * the behavior is consistent with the randomly chosen incoming payload type,
      as described in their respective sections.

### client_streaming

This test verifies that client-only streaming succeeds.

Server features:
* [StreamingInputCall][]
* [Compressable Payload][]

Procedure:
 1. Client calls StreamingInputCall
 2. Client sends:

    ```
    {
      payload:{
        body: 27182 bytes of zeros
      }
    }
    ```

 3. Client then sends:

    ```
    {
      payload:{
        body: 8 bytes of zeros
      }
    }
    ```

 4. Client then sends:

    ```
    {
      payload:{
        body: 1828 bytes of zeros
      }
    }
    ```

 5. Client then sends:

    ```
    {
      payload:{
        body: 45904 bytes of zeros
      }
    }
    ```

 6. Client half-closes

Client asserts:
* call was successful
* response aggregated_payload_size is 74922

### server_streaming

This test verifies that server-only streaming succeeds.

Server features:
* [StreamingOutputCall][]
* [Compressable Payload][]

Procedure:
 1. Client calls StreamingOutputCall with:

    ```
    {
      response_type:COMPRESSABLE
      response_parameters:{
        size: 31415
      }
      response_parameters:{
        size: 9
      }
      response_parameters:{
        size: 2653
      }
      response_parameters:{
        size: 58979
      }
    }
    ```

Client asserts:
* call was successful
* exactly four responses
* response payloads are COMPRESSABLE
* response payload bodies are sized (in order): 31415, 9, 2653, 58979
* clients are free to assert that the response payload body contents are zero
  and comparing the entire response messages against golden responses

### server_compressed_streaming

This test verifies that server-only compressed streaming succeeds.

Server features:
* [StreamingOutputCall][]
* [Compressable Payload][]
* [Uncompressable Payload][]
* [Random Payload][]


Procedure:
 1. Client calls StreamingOutputCall with:

    ```
    {
      response_compression: <one of {NONE, GZIP, DEFLATE}>
      response_type:COMPRESSABLE
      response_parameters:{
        size: 31415
      }
      response_parameters:{
        size: 9
      }
      response_parameters:{
        size: 2653
      }
      response_parameters:{
        size: 58979
      }
    }
    ```

    Client asserts:
    * call was successful
    * exactly four responses
    * response payloads are COMPRESSABLE
    * response compression is consistent with the requested one.
    * if `response_compression == NONE`, the response MUST NOT have the
      compressed message flag set.
    * if `response_compression != NONE`, the response MUST have the compressed
      message flag set.
    * response payload bodies are sized (in order): 31415, 9, 2653, 58979
    * clients are free to assert that the response payload body contents are
      zero and comparing the entire response messages against golden responses


 2. Client calls StreamingOutputCall with:

    ```
    {
      response_compression: <one of {NONE, GZIP, DEFLATE}>
      response_type:UNCOMPRESSABLE
      response_parameters:{
        size: 31415
      }
      response_parameters:{
        size: 9
      }
      response_parameters:{
        size: 2653
      }
      response_parameters:{
        size: 58979
      }
    }
    ```

    Client asserts:
    * call was successful
    * exactly four responses
    * response payloads are UNCOMPRESSABLE
    * response compressions are consistent with the requested one.
    * the responses MUST NOT have the compressed message flag set.
    * response payload bodies are sized (in order): 31415, 9, 2653, 58979
    * clients are free to assert that the body of the responses are identical to
      the golden uncompressable data at `test/cpp/interop/rnd.dat`.


 3. Client calls StreamingOutputCall with:

    ```
    {
      response_compression: <one of {NONE, GZIP, DEFLATE}>
      response_type:RANDOM
      response_parameters:{
        size: 31415
      }
      response_parameters:{
        size: 9
      }
      response_parameters:{
        size: 2653
      }
      response_parameters:{
        size: 58979
      }
    }
    ```

    Client asserts:
    * call was successful
    * response payload type is either COMPRESSABLE or UNCOMPRESSABLE
    * the behavior is consistent with the randomly chosen incoming payload type,
      as described in their respective sections.

### ping_pong

This test verifies that full duplex bidi is supported.

Server features:
* [FullDuplexCall][]
* [Compressable Payload][]

Procedure:
 1. Client calls FullDuplexCall with:

    ```
    {
      response_type: COMPRESSABLE
      response_parameters:{
        size: 31415
      }
      payload:{
        body: 27182 bytes of zeros
      }
    }
    ```

 2. After getting a reply, it sends:

    ```
    {
      response_type: COMPRESSABLE
      response_parameters:{
        size: 9
      }
      payload:{
        body: 8 bytes of zeros
      }
    }
    ```

 3. After getting a reply, it sends:

    ```
    {
      response_type: COMPRESSABLE
      response_parameters:{
        size: 2653
      }
      payload:{
        body: 1828 bytes of zeros
      }
    }
    ```

 4. After getting a reply, it sends:

    ```
    {
      response_type: COMPRESSABLE
      response_parameters:{
        size: 58979
      }
      payload:{
        body: 45904 bytes of zeros
      }
    }
    ```

 5. After getting a reply, client half-closes

Client asserts:
* call was successful
* exactly four responses
* response payloads are COMPRESSABLE
* response payload bodies are sized (in order): 31415, 9, 2653, 58979
* clients are free to assert that the response payload body contents are zero
  and comparing the entire response messages against golden responses

### empty_stream

This test verifies that streams support having zero-messages in both
directions.

Server features:
* [FullDuplexCall][]

Procedure:
 1. Client calls FullDuplexCall and then half-closes

Client asserts:
* call was successful
* exactly zero responses

### compute_engine_creds

This test is only for cloud-to-prod path.

This test verifies unary calls succeed in sending messages while using Service
Credentials from GCE metadata server. The client instance needs to be created
with desired oauth scope.

The test uses `--default_service_account` with GCE service account email and
`--oauth_scope` with the OAuth scope to use. For testing against
grpc-test.sandbox.google.com, "https://www.googleapis.com/auth/xapi.zoo" should
be passed in as `--oauth_scope`.

Server features:
* [UnaryCall][]
* [Compressable Payload][]
* [Echo Authenticated Username][]
* [Echo OAuth Scope][]

Procedure:
 1. Client configures channel to use GCECredentials
 2. Client calls UnaryCall on the channel with:

    ```
    {
      response_type: COMPRESSABLE
      response_size: 314159
      payload:{
        body: 271828 bytes of zeros
      }
      fill_username: true
      fill_oauth_scope: true
    }
    ```

Client asserts:
* call was successful
* received SimpleResponse.username equals the value of `--default_service_account` flag
* received SimpleResponse.oauth_scope is in `--oauth_scope`
* response payload body is 314159 bytes in size
* clients are free to assert that the response payload body contents are zero
  and comparing the entire response message against a golden response

### jwt_token_creds

This test is only for cloud-to-prod path.

This test verifies unary calls succeed in sending messages while using JWT
token (created by the project's key file)

Test caller should set flag `--service_account_key_file` with the
path to json key file downloaded from
https://console.developers.google.com.

Server features:
* [UnaryCall][]
* [Compressable Payload][]
* [Echo Authenticated Username][]
* [Echo OAuth Scope][]

Procedure:
 1. Client configures the channel to use JWTTokenCredentials
 2. Client calls UnaryCall with:

    ```
    {
      response_type: COMPRESSABLE
      response_size: 314159
      payload:{
        body: 271828 bytes of zeros
      }
      fill_username: true
    }
    ```

Client asserts:
* call was successful
* received SimpleResponse.username is not empty and is in the json key file used
by the auth library. The client can optionally check the username matches the
email address in the key file.
* response payload body is 314159 bytes in size
* clients are free to assert that the response payload body contents are zero
  and comparing the entire response message against a golden response

### oauth2_auth_token

This test is only for cloud-to-prod path and some implementations may run
in GCE only.

This test verifies unary calls succeed in sending messages using an OAuth2 token
that is obtained out of band. For the purpose of the test, the OAuth2 token is
actually obtained from a service account credentials or GCE credentials via the
language-specific authorization library.

The difference between this test and the other auth tests is that it
first uses the authorization library to obtain an authorization token.

The test
- uses the flag `--service_account_key_file` with the path to a json key file
downloaded from https://console.developers.google.com. Alternately, if using a
usable auth implementation, it may specify the file location in the environment
variable GOOGLE_APPLICATION_CREDENTIALS, *OR* if GCE credentials is used to
fetch the token, `--default_service_account` can be used to pass in GCE service
account email.
- uses the flag `--oauth_scope` for the oauth scope.  For testing against
grpc-test.sandbox.google.com, "https://www.googleapis.com/auth/xapi.zoo" should
be passed as the `--oauth_scope`.

Server features:
* [UnaryCall][]
* [Compressable Payload][]
* [Echo Authenticated Username][]
* [Echo OAuth Scope][]

Procedure:
 1. Client uses the auth library to obtain an authorization token
 2. Client configures the channel to use AccessTokenCredentials with the access token obtained in step 1
 3. Client calls UnaryCall with the following message

    ```
    {
      fill_username: true
      fill_oauth_scope: true
    }
    ```

Client asserts:
* call was successful
* received SimpleResponse.username is valid. Depending on whether a service
account key file or GCE credentials was used, client should check against the
json key file or GCE default service account email.
* received SimpleResponse.oauth_scope is in `--oauth_scope`

### per_rpc_creds

Similar to the other auth tests, this test is only for cloud-to-prod path.

This test verifies unary calls succeed in sending messages using a JWT
credentials set on the RPC.

The test
- uses the flag `--service_account_key_file` with the path to a json key file
downloaded from https://console.developers.google.com. Alternately, if using a
usable auth implementation, it may specify the file location in the environment
variable GOOGLE_APPLICATION_CREDENTIALS

Server features:
* [UnaryCall][]
* [Compressable Payload][]
* [Echo Authenticated Username][]
* [Echo OAuth Scope][]

Procedure:
 1. Client configures the channel with just SSL credentials
 2. Client calls UnaryCall, setting per-call credentials to
    JWTTokenCredentials. The request is the following message

    ```
    {
      fill_username: true
    }
    ```

Client asserts:
* call was successful
* received SimpleResponse.username is not empty and is in the json key file used
by the auth library. The client can optionally check the username matches the
email address in the key file.


### custom_metadata

This test verifies that custom metadata in either binary or ascii format can be
sent as initial-metadata by the client and as both initial- and trailing-metadata
by the server.

Server features:
* [UnaryCall][]
* [FullDuplexCall][]
* [Compressable Payload][]
* [Echo Metadata][]

Procedure:
 1. The client attaches custom metadata with the following keys and values:

    ```
    key: "x-grpc-test-echo-initial", value: "test_initial_metadata_value"
    key: "x-grpc-test-echo-trailing-bin", value: 0xababab
    ```

    to a UnaryCall with request:

    ```
    {
      response_type: COMPRESSABLE
      response_size: 314159
      payload:{
        body: 271828 bytes of zeros
      }
    }
    ```

 2. The client attaches custom metadata with the following keys and values:

    ```
    key: "x-grpc-test-echo-initial", value: "test_initial_metadata_value"
    key: "x-grpc-test-echo-trailing-bin", value: 0xababab
    ```

    to a FullDuplexCall with request:

    ```
    {
      response_type: COMPRESSABLE
      response_size: 314159
      payload:{
        body: 271828 bytes of zeros
      }
    }
    ```

    and then half-closes

Client asserts:
* call was successful
* metadata with key `"x-grpc-test-echo-initial"` and value
  `"test_initial_metadata_value"`is received in the initial metadata for calls
  in Procedure steps 1 and 2.
* metadata with key `"x-grpc-test-echo-trailing-bin"` and value `0xababab` is
  received in the trailing metadata for calls in Procedure steps 1 and 2.



### status_code_and_message

This test verifies unary calls succeed in sending messages, and propagate back
status code and message sent along with the messages.

Server features:
* [UnaryCall][]
* [FullDuplexCall][]
* [Echo Status][]

Procedure:
 1. Client calls UnaryCall with:

    ```
    {
      response_status:{
        code: 2
        message: "test status message"
      }
    }
    ```

 2. Client calls FullDuplexCall with:

    ```
    {
      response_status:{
        code: 2
        message: "test status message"
      }
    }
    ```

    and then half-closes


Client asserts:
* received status code is the same as the sent code for both Procedure steps 1
  and 2
* received status message is the same as the sent message for both Procedure
  steps 1 and 2

### unimplemented_method

Status: Ready for implementation. Blocking beta.

This test verifies calling unimplemented RPC method returns the UNIMPLEMENTED status code.

Server features:
N/A

Procedure:
* Client calls `grpc.testing.UnimplementedService/UnimplementedCall` with an
  empty request (defined as `grpc.testing.Empty`):

    ```
    {
    }
    ```

Client asserts:
* received status code is 12 (UNIMPLEMENTED)
* received status message is empty or null/unset

### cancel_after_begin

This test verifies that a request can be cancelled after metadata has been sent
but before payloads are sent.

Server features:
* [StreamingInputCall][]

Procedure:
 1. Client starts StreamingInputCall
 2. Client immediately cancels request

Client asserts:
* Call completed with status CANCELLED

### cancel_after_first_response

This test verifies that a request can be cancelled after receiving a message
from the server.

Server features:
* [FullDuplexCall][]
* [Compressable Payload][]

Procedure:
 1. Client starts FullDuplexCall with

    ```
    {
      response_type: COMPRESSABLE
      response_parameters:{
        size: 31415
      }
      payload:{
        body: 27182 bytes of zeros
      }
    }
    ```

 2. After receiving a response, client cancels request

Client asserts:
* Call completed with status CANCELLED

### timeout_on_sleeping_server

This test verifies that an RPC request whose lifetime exceeds its configured
timeout value will end with the DeadlineExceeded status.

Server features:
* [FullDuplexCall][]

Procedure:
 1. Client calls FullDuplexCall with the following request and sets its timeout
    to 1ms

    ```
    {
      payload:{
        body: 27182 bytes of zeros
      }
    }
    ```

 2. Client waits

Client asserts:
* Call completed with status DEADLINE_EXCEEDED.

### concurrent_large_unary

Status: TODO

Client performs 1000 large_unary tests in parallel on the same channel.

### Flow control. Pushback at client for large messages (TODO: fix name)

Status: TODO

This test verifies that a client sending faster than a server can drain sees
pushback (i.e., attempts to send succeed only after appropriate delays).

### TODO Tests

#### High priority:

Propagation of status code and message (yangg)

Multiple thousand simultaneous calls on same Channel (ctiller)

Metadata: client headers, server headers + trailers, binary+ascii

#### Normal priority:

Cancel before start (ctiller)

Cancel after sent first message (ctiller)

Cancel after received headers (ctiller)

Timeout but completed before expire (zhaoq)

Multiple thousand simultaneous calls timeout on same Channel (ctiller)

#### Lower priority:

Flow control. Pushback at client for large messages (abhishek)

Flow control. Pushback at server for large messages (abhishek)

Going over max concurrent streams doesn't fail (client controls itself)
(abhishek)

RPC method not implemented (yangg)

Multiple thousand simultaneous calls on different Channels (ctiller)

Failed TLS hostname verification (ejona?)

Large amount of headers to cause CONTINUATIONs; 63K of 'X's, all in one header.

#### To priorize:

Start streaming RPC but don't send any requests, server responds

### Postponed Tests

Resilience to buggy servers: These tests would verify that a client application
isn't affected negatively by the responses put on the wire by a buggy server
(e.g. the client library won't make the application crash).

Reconnect after transport failure

Reconnect backoff

Fuzz testing


Server
------

Servers implement various named features for clients to test with. Server
features are orthogonal. If a server implements a feature, it is always
available for clients. Names are simple descriptions for developer
communication and tracking.

Servers should accept these arguments:

* --port=PORT

    * The port to listen on. For example, "8080"

* --use_tls=BOOLEAN

    * Whether to use a plaintext or encrypted connection

Servers must support TLS with ALPN. They should use
[server1.pem](https://github.com/grpc/grpc/blob/master/src/core/tsi/test_creds/server1.pem)
for their certificate.

### EmptyCall
[EmptyCall]: #emptycall

Server implements EmptyCall which immediately returns the empty message.

### UnaryCall
[UnaryCall]: #unarycall

Server implements UnaryCall which immediately returns a SimpleResponse with a
payload body of size SimpleRequest.response_size bytes and type as appropriate
for the SimpleRequest.response_type. If the server does not support the
response_type, then it should fail the RPC with INVALID_ARGUMENT.

### StreamingInputCall
[StreamingInputCall]: #streaminginputcall

Server implements StreamingInputCall which upon half close immediately returns
a StreamingInputCallResponse where aggregated_payload_size is the sum of all
request payload bodies received.

### StreamingOutputCall
[StreamingOutputCall]: #streamingoutputcall

Server implements StreamingOutputCall by replying, in order, with one
StreamingOutputCallResponses for each ResponseParameters in
StreamingOutputCallRequest. Each StreamingOutputCallResponses should have a
payload body of size ResponseParameters.size bytes, as specified by its
respective ResponseParameters. After sending all responses, it closes with OK.

### FullDuplexCall
[FullDuplexCall]: #fullduplexcall

Server implements FullDuplexCall by replying, in order, with one
StreamingOutputCallResponses for each ResponseParameters in each
StreamingOutputCallRequest. Each StreamingOutputCallResponses should have a
payload body of size ResponseParameters.size bytes, as specified by its
respective ResponseParameters. After receiving half close and sending all
responses, it closes with OK.

### Compressable Payload
[Compressable Payload]: #compressable-payload

When the client requests COMPRESSABLE payload, the response includes a payload
of the size requested containing all zeros and the payload type is
COMPRESSABLE.

### Uncompressable Payload
[Uncompressable Payload]: #uncompressable-payload

When the client requests UNCOMPRESSABLE payload, the response includes a payload
of the size requested containing uncompressable data and the payload type is
UNCOMPRESSABLE. A 512 kB dump from /dev/urandom is the current golden data,
stored at `test/cpp/interop/rnd.dat`

### Random Payload
[Random Payload]: #random-payload

When the client requests RANDOM payload, the response includes either a randomly
chosen COMPRESSABLE or UNCOMPRESSABLE payload. The data and the payload type
will be consistent with this choice.

### Echo Status
[Echo Status]: #echo-status
When the client sends a response_status in the request payload, the server closes
the stream with the status code and messsage contained within said response_status.
The server will not process any further messages on the stream sent by the client.
This can be used by clients to verify correct handling of different status codes and
associated status messages end-to-end.

### Echo Metadata
[Echo Metadata]: #echo-metadata
When the client sends metadata with the key `"x-grpc-test-echo-initial"` with its
request, the server sends back exactly this key and the corresponding value back to
the client as part of initial metadata. When the client sends metadata with the key
`"x-grpc-test-echo-trailing-bin"` with its request, the server sends back exactly this
key and the corresponding value back to the client as trailing metadata.

### Observe ResponseParameters.interval_us
[Observe ResponseParameters.interval_us]: #observe-responseparametersinterval_us

In StreamingOutputCall and FullDuplexCall, server delays sending a
StreamingOutputCallResponse by the ResponseParameters's interval_us for that
particular response, relative to the last response sent. That is, interval_us
acts like a sleep *before* sending the response and accumulates from one
response to the next.

Interaction with flow control is unspecified.

### Echo Auth Information

Status: Pending

#### Echo Authenticated Username
[Echo Authenticated Username]: #echo-authenticated-username

If a SimpleRequest has fill_username=true and that request was successfully
authenticated, then the SimpleResponse should have username filled with the
canonical form of the authenticated source. The canonical form is dependent on
the authentication method, but is likely to be a base 10 integer identifier or
an email address.

#### Echo OAuth scope
[Echo OAuth Scope]: #echo-oauth-scope

If a SimpleRequest has fill_oauth_scope=true and that request was successfully
authenticated via OAuth, then the SimpleResponse should have oauth_scope filled
with the scope of the method being invoked.

Although a general server-side feature, most test servers won't implement this
feature. The TLS server grpc-test.sandbox.google.com:443 supports this feature.
It requires at least the OAuth scope
`https://www.googleapis.com/auth/xapi.zoo` for authentication to succeed.

Discussion:

Ideally, this would be communicated via metadata and not in the
request/response, but we want to use this test in code paths that don't yet
fully communicate metadata.

