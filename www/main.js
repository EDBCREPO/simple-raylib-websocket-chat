window.addEventListener( "load", ()=>{

    let cli = new WebSocket( "ws://localhost:8000" );

    cli.onmessage = ({ data })=>{
        document.querySelector("#messages").innerHTML += `<div>${data}</div>`;
    }

    cli.onclose = ()=>{
        document.querySelector("h1").innerHTML = "Disconnected";
    }

    cli.onopen = ()=>{
        document.querySelector("h1").innerHTML = "Connected";
    }

    document.querySelector("#send").addEventListener( "click", ()=>{
        cli.send( document.querySelector("#message").value );
        document.querySelector("#message").value = "";
    });

});