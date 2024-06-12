<script lang="ts">
  import SpecifierArg from "./SpecifierArg.svelte";
  let format = "";
  let specifiers = [];
  let output = "";

  function submit() {
    let args = [];
    let inputs = document.getElementsByClassName("format-arg");
    for (let i = 0; i < inputs.length; i++) {
      console.log(inputs[i]);
      args.push(inputs[i]["value"]);
    }
    console.log(JSON.stringify({'format': format, "args": args}));
    let request = new Request('http://localhost:8081/format', {
      method: 'POST',
      mode: 'cors',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({'format': format, "args": args})
    });
    fetch(request)
      .then(async (response) => {
        console.log(response);
        let result = await response.json();
        if (result && result['output']) {
          output = result['output'].trim();
        }
      })
      .catch(error => console.error(error));
  }

  function updateSpecifiers() {
    specifiers = [];
    let i = 0;
    while (i < format.length) {
      if (format.charAt(i) === "%") {
        let j = i + 1;
        while (j < format.length && !isSpecifier(format.charAt(j))) {
          j++;
        }
        if (isSpecifier(format.charAt(j)) && format.charAt(j) !== "%") {
          specifiers.push(format.slice(i, j + 1));
        }
        i = j + 1;
      } else {
        i++;
      }
    }
  }

  function isSpecifier(c: string): boolean {
    switch (c) {
      case '%':
      case 'c':
      case 's':
      case 'd':
      case 'i':
      case 'o':
      case 'x':
      case 'X':
      case 'u':
      case 'f':
      case 'F':
      case 'e':
      case 'E':
      case 'a':
      case 'A':
      case 'g':
      case 'G':
      case 'n':
      case 'p':
        return true;
    }
    return false;
  }
</script>

<main>
  <div>
    <label>
      <span class="label-text">
        Format
      </span>
      <input bind:value={format} on:input={updateSpecifiers} class="text-mono"/>
    </label>
  </div>
  <div>
    {#each specifiers as specifier}
      <SpecifierArg specifier={specifier} />
    {/each}
  </div>
  <div>
    <button disabled={!format} type="submit" on:click={submit}>
      Submit
    </button>
  </div>
  {#if format && output}
  <div>
    <pre class="text-mono">{output}</pre>
  </div>
  {/if}
</main>
